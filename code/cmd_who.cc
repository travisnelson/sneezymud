//////////////////////////////////////////////////////////////////////////
//
// SneezyMUD - All rights reserved, SneezyMUD Coding Team
//
//    "cmd_who.cc" - the who command
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "stdsneezy.h"
#include "statistics.h"
#include "games.h"

void TBeing::parseTitle(char *arg, Descriptor *)
{
  strcpy(arg, getName());
  return;
}

void TPerson::parseTitle(char *arg, Descriptor *user)
{
  int flag = FALSE;
  if (!title) {
    strcpy(arg, getName());
    return;
  }

  strcpy(arg, nameColorString(this, user, title, &flag, COLOR_BASIC, FALSE).c_str());
  if (!flag &&
      colorString(this, user, title, NULL, COLOR_NONE, TRUE).find(getNameNOC(this).c_str()) ==
      string::npos)
    strcpy(arg, getName());  // did not specify a <n>

  // explicitely terminate it since players are sloppy
  strcat(arg, "<1>");

  return;
}

void Descriptor::menuWho() 
{
  TBeing *person;
  char buf[256];
  char buf2[256];
  char send[4096] = "\0";

  strcpy(send, "\n\r");

  for (person = character_list; person; person = person->next) {
    if (person->isPc() && person->polyed == POLY_TYPE_NONE) {
      if (dynamic_cast<TPerson *>(person) &&
          (person->getInvisLevel() < GOD_LEVEL1)) {
        person->parseTitle(buf, this);

        sprintf(buf2, "%s", colorString(person, this, buf, NULL, COLOR_BASIC, FALSE).c_str());
        strcat(buf2, "\n\r");
        strcat(send, buf2);
      }
    }
  }
  strcat(send, "\n\r");
  writeToQ(send);
  writeToQ("[Press return to continue]\n\r");
}

static const string getWizDescriptLev(const TBeing *ch)
{
  if (ch->hasWizPower(POWER_WIZARD))
    return "creator";
  else if (ch->hasWizPower(POWER_GOD)) {
    if(ch->getSex() == SEX_FEMALE)
      return "godess ";
    else
      return "  god  ";
  }
  else if (ch->hasWizPower(POWER_BUILDER))
    return "demigod";
  else
    return "BUG ME!";
}

static const string getWhoLevel(const TBeing *ch, TBeing *p)
{
  char tempbuf[256];
  char colorBuf[256] = "\0";

  if (p->hasWizPower(POWER_WIZARD))
    strcpy(colorBuf, ch->purple());
  else if (p->hasWizPower(POWER_GOD))
    strcpy(colorBuf, ch->red());
  else if (p->hasWizPower(POWER_BUILDER))
    strcpy(colorBuf, ch->cyan());

  // Do it this way so you get the default-titles also.
  if (p && p->GetMaxLevel() > MAX_MORT) {
    string str = p->msgVariables(MSG_IMM_TITLE);
    unsigned int len = str.size();
    unsigned int padding = 14-len;
    unsigned int frontpadding = padding/2;
    for (unsigned int iter = 0; iter < frontpadding; iter++)
      str.insert(0, " ");
    
    sprintf(tempbuf, "%sLevel:[%-14s%s][%s] %s",
            colorBuf, str.c_str(),
            colorBuf, getWizDescriptLev(p).c_str(), ch->norm());
  } else {
    string tmpstring;

    if(p->isPlayerAction(PLR_ANONYMOUS) && !ch->isImmortal()){
      tmpstring = "Anonymous";
    } else {
      sprintf(tempbuf, "%-5s Lev %2d", p->getProfAbbrevName(), p->GetMaxLevel());
      tmpstring += tempbuf;
    }

    while (tmpstring.length() < 13)
      tmpstring = " " + tmpstring + " ";
    if (tmpstring.length() < 14)
      tmpstring += " ";

    sprintf(tempbuf, "Level:[%s] ", tmpstring.c_str());
    TFaction *f = NULL;
    if((f = p->newfaction()) && TestCode5) {
      if (f->ID && (IS_SET(f->flags, FACT_ACTIVE) || ch->newfaction() == p->newfaction() || ch->isImmortal()) &&
	  (!IS_SET(f->flags, FACT_HIDDEN) || ch->newfaction() == p->newfaction() || ch->isImmortal()) &&
	  (!p->isImmortal() || ch->isImmortal())) {
	sprintf(tempbuf, "%s %s[<1>%s%s]<1>", tempbuf,
		heraldcodes[p->newfaction()->colors[0]],
		p->newfaction()->getName(),
		heraldcodes[p->newfaction()->colors[0]]);
      }
    }
       
  }

  return tempbuf;
}

void TBeing::doWho(const char *argument)
{
  TBeing *k, *p;
  char buf[1024] = "\0\0\0";
  string bufstr;
  int listed = 0, lcount, l;
  unsigned int count;
  char arg[1024], tempbuf[1024];
  char tString[256];
  string sb;
  int which1 = 0;
  int which2 = 0;

  for (; isspace(*argument); argument++);

  sb += "Players: (Add -? for online help)\n\r--------\n\r";
  lcount = count = 0;

  if (!*argument || 
       ((sscanf(argument, "%d %d", &which1, &which2) == 2) && 
          which1 > 0 && which2 > 0) ||
       ((sscanf(argument, "%d %d", &which1, &which2) == 1) && 
          which1 > 0  && (which2 = MAX_IMMORT))) {
    // plain old 'who' command 
    // who <level>      level2 assigned to 60
    // who <level> <level2>
    for (p = character_list; p; p = p->next) {
      if (p->isPc() && p->polyed == POLY_TYPE_NONE) {
        if (dynamic_cast<TPerson *>(p)) {
          if (canSeeWho(p) && (!*argument || ((!p->isPlayerAction(PLR_ANONYMOUS) || isImmortal()) && p->GetMaxLevel() >= which1 && p->GetMaxLevel() <= which2))){
            count++;

            p->parseTitle(buf, desc);
	    bufstr=buf;
            if (!*argument) {
              if (p->isPlayerAction(PLR_SEEKSGROUP))
		bufstr += "   (Seeking Group)";

              if (p->isPlayerAction(PLR_NEWBIEHELP))
		bufstr += "   (Newbie-Helper)";

	      bufstr += "\n\r";
            } else {
	      bufstr += "   " + getWhoLevel(this, p);

              if (p->isPlayerAction(PLR_SEEKSGROUP))
		bufstr += "   (Seeking Group)";

              if (p->isPlayerAction(PLR_NEWBIEHELP))
		bufstr += "   (Newbie-Helper)";

	      bufstr += "\n\r";
            }

            if (isImmortal() && p->isLinkdead()) {
            } else {
              snprintf(tempbuf, 1023, "%s%s",
                  (p->polyed == POLY_TYPE_SWITCH ?  "(switched) " : ""), bufstr.c_str());
              sb += tempbuf;
            }
          }
        } else if (isImmortal()) {
// only immortals will see this to provide them some concealment
          if (canSeeWho(p) && 
              (!*argument || 
                (p->GetMaxLevel() >= which1 && p->GetMaxLevel() <= which2)) &&
              IS_SET(p->specials.act, ACT_POLYSELF)) {
            count++;
            strncpy(tempbuf, pers(p), 1023);
            snprintf(buf, 1023, "%s (polymorphed)\n\r", cap(tempbuf));
            sb += buf;
          } else if (canSeeWho(p) &&
                (!*argument || 
                (p->GetMaxLevel() >= which1 && p->GetMaxLevel() <= which2)) &&
                     IS_SET(p->specials.act, ACT_DISGUISED)) {
            count++;
            strncpy(tempbuf, pers(p), 1023);
            snprintf(buf, 1023, "%s (disguised thief)\n\r", cap(tempbuf));
            sb += buf;
          }
        }
      }
    }
    accStat.max_player_since_reboot = max(accStat.max_player_since_reboot, count);
    snprintf(buf, 1023, "\n\rTotal Players : [%d] Max since last reboot : [%d] Avg Players : [%.1f]\n\r", count, accStat.max_player_since_reboot, stats.useage_iters ? (float) stats.num_users / stats.useage_iters : 0);
    sb += buf;
    if (desc)
      desc->page_string(sb.c_str(), SHOWNOW_NO, ALLOWREP_YES);

    return;
  } else {
    argument = one_argument(argument, arg);
    if (*arg == '-') {
      if (strchr(arg, '?')) {
        if (isImmortal()) {
          sb += "[-] [i]idle [l]levels [q]quests [h]hit/mana/move/lf\n\r";
          sb += "[-] [z]seeks-group [p]groups [y]currently-not-grouped\n\r";
          sb += "[-] [d]linkdead [g]God [b]Builders [o]Mort [s]stats [f]action\n\r";
          sb += "[-] [1]Mage[2]Cleric[3]War[4]Thief[5]Deikhan[6]Monk[7]Ranger[8]Shaman\n\r";
          sb += "[-] [e]elf [t]hobbit [n]gnome [u]human [r]ogre [w]dwarven\n\r\n\r";
	  sb += "[-] [x]Perma Death\n\r";

          if (hasWizPower(POWER_WIZARD))
            sb += "[-] [a]ccount\n\r";

          sb += "\n\r";
        } else {
          sb += "[-] [q]quests [g]god [b]builder [o]mort [f]faction\n\r";
          sb += "[-] [z]seeks-group [p]groups [y]currently-not-grouped\n\r";
          sb += "[-] [e]elf [t]hobbit [n]gnome [u]human [r]ogre [w]dwarven\n\r\n\r";
          sb += "[-] [1]Mage[2]Cleric[3]War[4]Thief[5]Deikhan[6]Monk[7]Ranger[8]Shaman\n\r";
	  sb += "[-] [x]Perma Death\n\r";
        }
        if (desc)
          desc->page_string(sb.c_str(), SHOWNOW_NO, ALLOWREP_YES);
        return;
      }
      bool level, statsx, iPoints, quest, idle, align, group;
      for (p = character_list; p; p = p->next) {
        align = level = statsx = idle = iPoints = quest = group = FALSE;
        if (dynamic_cast<TPerson *>(p) && canSeeWho(p)) {
          count++;
          if (p->isLinkdead())
            lcount++;

          if ((canSeeWho(p) &&
              (!strchr(arg, 'g') || (p->GetMaxLevel() >= GOD_LEVEL1)) &&
              (!strchr(arg, 'b') || (p->GetMaxLevel() >= GOD_LEVEL1)) &&
              (!strchr(arg, 'q') || (p->inQuest())) &&
              (!strchr(arg, 'o') || (p->GetMaxLevel() <= MAX_MORT)) &&
              (!strchr(arg, 'z') || (p->isPlayerAction(PLR_SEEKSGROUP))) &&
              (!strchr(arg, 'p') || (p->isAffected(AFF_GROUP) && !p->master && p->followers)) &&
              (!strchr(arg, 'y') || (!p->isAffected(AFF_GROUP) && !p->isImmortal())) &&
              (!strchr(arg, '1') || (p->hasClass(CLASS_MAGIC_USER) && (isImmortal() || !p->isPlayerAction(PLR_ANONYMOUS)))) &&
              (!strchr(arg, '2') || (p->hasClass(CLASS_CLERIC) && (isImmortal() || !p->isPlayerAction(PLR_ANONYMOUS)))) &&
              (!strchr(arg, '3') || (p->hasClass(CLASS_WARRIOR) && (isImmortal() || !p->isPlayerAction(PLR_ANONYMOUS)))) &&
              (!strchr(arg, '4') || (p->hasClass(CLASS_THIEF) && (isImmortal() || !p->isPlayerAction(PLR_ANONYMOUS)))) &&
              (!strchr(arg, '5') || (p->hasClass(CLASS_DEIKHAN) && (isImmortal() || !p->isPlayerAction(PLR_ANONYMOUS)))) &&
              (!strchr(arg, '6') || (p->hasClass(CLASS_MONK) && (isImmortal() || !p->isPlayerAction(PLR_ANONYMOUS)))) &&
              (!strchr(arg, '7') || (p->hasClass(CLASS_RANGER) && (isImmortal() || !p->isPlayerAction(PLR_ANONYMOUS)))) &&
              (!strchr(arg, '8') || (p->hasClass(CLASS_SHAMAN) && (isImmortal() || !p->isPlayerAction(PLR_ANONYMOUS)))) &&
              (!strchr(arg, 'd') || (p->isLinkdead() && isImmortal())) &&
              (!strchr(arg, 'e') || p->getRace() == RACE_ELVEN) &&
              (!strchr(arg, 'n') || p->getRace() == RACE_GNOME) &&
              (!strchr(arg, 'u') || p->getRace() == RACE_HUMAN) &&
              (!strchr(arg, 'w') || p->getRace() == RACE_DWARF) &&
              (!strchr(arg, 'r') || p->getRace() == RACE_OGRE) &&
              (!strchr(arg, 't') || p->getRace() == RACE_HOBBIT) &&
	      (!strchr(arg, 'x') || p->hasQuestBit(TOG_PERMA_DEATH_CHAR)))) {
            if (p->isLinkdead() && isImmortal())
              snprintf(buf, 1023, "[%-12s] ", pers(p));
            else if (p->polyed == POLY_TYPE_SWITCH && isImmortal())
              snprintf(buf, 1023, "[%-12s] (switched) ", pers(p));
            else if (dynamic_cast<TMonster *>(p) &&
                     (p->specials.act & ACT_POLYSELF))
              snprintf(buf, 1023, "(%-14s) ", pers(p));
            else 
              snprintf(buf, 1023, "%-11s ", pers(p));
            listed++;
            for (l = 1; l <= (int) strlen(arg); l++) {
              switch (arg[l]) {
                case 'p':
                  // we trapped only group leaders above...
                  if (!group) {
                    TBeing *ch;
                    followData *f;
                    for (f = p->followers; f; f = f->next) {
                      ch = f->follower;
                      if (!ch->isPc())
                        continue;
                      if (!canSeeWho(ch))
                        continue;
                      if (ch->isLinkdead() && isImmortal())
                        snprintf(buf, 1023, "[%-12s] ", pers(ch));
                      else if (ch->polyed == POLY_TYPE_SWITCH && isImmortal())
                        snprintf(buf, 1023, "[%-12s] (switched) ", pers(ch));
                      else if (dynamic_cast<TMonster *>(ch) &&
                               (ch->specials.act & ACT_POLYSELF))
                        snprintf(buf + strlen(buf), 1023-strlen(buf), "(%-14s) ", pers(ch));
                      else if (ch->isPlayerAction(PLR_ANONYMOUS) && !isImmortal())
                        snprintf(buf + strlen(buf), 1023-strlen(buf), "%-11s (???) ", pers(ch));
                      else
                        snprintf(buf + strlen(buf), 1023-strlen(buf), "%-11s (L%d) ", pers(ch), ch->GetMaxLevel());
                    }

                    group = true;
                  }
                  break;
                case 'i':
                  if (!idle) {
                    if (isImmortal())
                      snprintf(buf + strlen(buf), 1023-strlen(buf), "Idle:[%-3d] ", p->getTimer());
                  }
                  idle = TRUE;
                  break;
                case 'l':
                case 'y':
                  if (!level) {
                    strncat(buf, getWhoLevel(this, p).c_str(), 1023-strlen(buf));
                    if (p->isPlayerAction(PLR_SEEKSGROUP))
                      snprintf(buf + strlen(buf), 1023-strlen(buf), "   (Seeking Group)");

                    if (p->isPlayerAction(PLR_NEWBIEHELP))
                      snprintf(buf + strlen(buf), 1023-strlen(buf), "   (Newbie-Helper)");
                  }
                  level = TRUE;
                  break;
                case 'g':
                case 'b':
                  // canSeeWho already separated out invisLevel > my own
                  // only a god can go invis, mortals technically have
                  // invisLevel if they are linkdead, ignore that though
                  if (p->getInvisLevel() > MAX_MORT)
                    snprintf(buf + strlen(buf), 1023-strlen(buf), "  (invis %d)  ",
                        p->getInvisLevel());
                  break;
                case 'h':
                  if (!iPoints) {
                    if (isImmortal())
                      if (p->hasClass(CLASS_CLERIC)||p->hasClass(CLASS_DEIKHAN))
                        sprintf(buf + strlen(buf), "Hit:[%-3d] Pty:[%-.2f] Move:[%-3d], Talens:[%-8d], Bank:[%-8d]",
                              p->getHit(), p->getPiety(), p->getMove(), p->getMoney(), p->getBank());
                      else if (p->hasClass(CLASS_SHAMAN))
                        sprintf(buf + strlen(buf), "Hit:[%-3d] LF:[%-4d] Move:[%-3d], Talens:[%-8d], Bank:[%-8d]",
                              p->getHit(), p->getLifeforce(), p->getMove(), p->getMoney(), p->getBank());
                      else
                          sprintf(buf + strlen(buf), "Hit:[%-3d] Mana:[%-3d] Move:[%-3d], Talens:[%-8d], Bank:[%-8d]",
                              p->getHit(), p->getMana(), p->getMove(), p->getMoney(), p->getBank());
                  }
                  iPoints = TRUE;
                  break;
                case 'f':
                  if (!align) {
                    // show factions of everyone to immorts
                    // mortal version will show non-imms that are in same fact
                    if(TestCode5) {
		      TFaction *f = NULL;
		      if((f = p->newfaction()) && TestCode5) {
			if (f->ID && (IS_SET(f->flags, FACT_ACTIVE) || newfaction()== p->newfaction()||isImmortal()) &&
			    (!IS_SET(f->flags, FACT_HIDDEN) || newfaction() == p->newfaction() || isImmortal()) &&
			    (!p->isImmortal() || isImmortal())) {
			  sprintf(buf + strlen(buf), "%s[<1>%s%s]<1>",
				  heraldcodes[p->newfaction()->colors[0]],
				  p->newfaction()->getName(),
				  heraldcodes[p->newfaction()->colors[0]]);
			  if(!IS_SET(f->flags, FACT_HIDE_RANKS) || newfaction() == p->newfaction()
			     || isImmortal()) 
			  sprintf(buf + strlen(buf), " %s[<1>%s%s]<1>",
				  heraldcodes[p->newfaction()->colors[1]],
				  p->rank(),
                                  heraldcodes[p->newfaction()->colors[1]]);
			}
		      }
		      

		    } else {
		      if ((getFaction()==p->getFaction() &&
			   p->GetMaxLevel() <= MAX_MORT) || isImmortal()) {
#if FACTIONS_IN_USE
			sprintf(buf + strlen(buf), "[%s] %5.2f%%", 
				FactionInfo[p->getFaction()].faction_name,
				p->getPerc());
#else
			sprintf(buf + strlen(buf), "[%s]", 
				FactionInfo[p->getFaction()].faction_name);
#endif
		      }
		    }
		  }
                  align = TRUE;
                  break;
                case 's':
                  if (!statsx) {
                    if (isImmortal())
                      sprintf(buf + strlen(buf), "\n\r\t[St:%-3d Br:%-3d Co:%-3d De:%-3d Ag:%-3d In:%-3d Wi:%-3d Fo:%-3d Pe:%-3d Ch:%-3d Ka:%-3d Sp:%-3d]",
                        p->curStats.get(STAT_STR),
                        p->curStats.get(STAT_BRA),
                        p->curStats.get(STAT_CON),
                        p->curStats.get(STAT_DEX),
                        p->curStats.get(STAT_AGI),
                        p->curStats.get(STAT_INT),
        		p->curStats.get(STAT_WIS),
			p->curStats.get(STAT_FOC),
			p->curStats.get(STAT_PER),
			p->curStats.get(STAT_CHA),
			p->curStats.get(STAT_KAR),
			p->curStats.get(STAT_SPE));
                  }
                  statsx = TRUE;
                  break;
                case 'q':
                  if (!quest) {
                    if (p->isPlayerAction(PLR_SOLOQUEST))
                      sprintf(buf + strlen(buf), " (%sSOLO QUEST%s)", red(), norm());
                    
                    if (p->isPlayerAction(PLR_GRPQUEST))
                      sprintf(buf + strlen(buf), " (%sGROUP QUEST%s)", blue(), norm());
                  }
                  quest = TRUE;
                  break;
                case 'a':
                  if (isImmortal() && hasWizPower(POWER_WIZARD)) {
                    if (p->desc && p->desc->account)
                      sprintf(tString, " Account[%s]", p->desc->account->name);
                    else
                      sprintf(tString, " Account[Unknown]");

                    strcat(buf, tString);
                  }
                  break;
                default:
                  break;
              }        // end of switch statement 
            }        // end of for-loop 
            strcat(buf, "\n\r");
            sb += buf;
          }        // end of 'should I skip this fool' if-statement 
        }        // end of !NPC(p) loop 
      }                // end of 'step through the character list loop 
    } else {
      // 'who playername' command 
      int c = 0;
      for (k = character_list; k; k = k->next) {
        if (!k->isPc() || !isname(arg, k->name) || !canSee(k)) 
          continue;
 
        c++;
        *buf = '\0';
        k->parseTitle(buf, desc);
        strcat(buf, "    ");
        strcat(buf, getWhoLevel(this, k).c_str());
        if (k->isPlayerAction(PLR_SEEKSGROUP))
          sprintf(buf + strlen(buf), "   (Seeking Group)");

        if (k->isLinkdead() && isImmortal())
          strcat(buf, "   (link-dead)");

        if (k->polyed == POLY_TYPE_SWITCH && isImmortal())
          strcat(buf, "   (switched)");

        if (k->isPlayerAction(PLR_NEWBIEHELP))
          sprintf(buf + strlen(buf), "   (Newbie-Helper)");

        strcat(buf, "\n\r");
        sb += buf;
      }
      if (!c)
        sb += "No one logged in with that name.\n\r";

      if (desc)
        desc->page_string(sb.c_str(), SHOWNOW_NO, ALLOWREP_YES);
      return;
    }
  }
  accStat.max_player_since_reboot = max(accStat.max_player_since_reboot, count);
  if (isImmortal()) {
    if (!listed)
      sprintf(buf, "\n\rTotal players / Link dead [%d/%d] (%2.0f%%)\n\rMax since Reboot [%d]  Avg Players : [%.1f]\n\r",
           count, lcount, ((double) lcount / (int) count) * 100, 
           accStat.max_player_since_reboot,
           stats.useage_iters ? (float) stats.num_users / stats.useage_iters : 0);
    else
      sprintf(buf, "\n\rTotal players / Link dead [%d/%d] (%2.0f%%)\n\rNumber Listed: %d  Max since Reboot [%d]  Avg Players : [%.1f]\n\r", 
           count, lcount, ((double) lcount / (int) count) * 100, listed,
           accStat.max_player_since_reboot,
           stats.useage_iters ? (float) stats.num_users / stats.useage_iters : 0);
  } else {
    sprintf(buf, "\n\rTotal Players : [%d] Max since last reboot : [%d] Avg Players : [%.1f]\n\r", count, accStat.max_player_since_reboot, stats.useage_iters ? (float) stats.num_users / stats.useage_iters : 0);
  }
  sb += buf;
  if (desc)
    desc->page_string(sb.c_str(), SHOWNOW_NO, ALLOWREP_YES);
  return;
}

void TBeing::doWhozone()
{
  Descriptor *d;
  TRoom *rp = NULL;
  char buf[256];
  TBeing *person = NULL;
  int count = 0;

  sendTo("Players:\n\r--------\n\r");
  for (d = descriptor_list; d; d = d->next) {
    if (!d->connected && canSee(d->character) &&
        (rp = real_roomp((person = (d->original ? d->original : d->character))->in_room)) &&
        (rp->getZoneNum() == roomp->getZoneNum())) {
      sprintf(buf, "%-25s - %s ", person->getName(), rp->name);
      if (GetMaxLevel() > MAX_MORT)
        sprintf(buf + strlen(buf), "[%d]", person->in_room);
      strcat(buf, "\n\r");
      sendTo(COLOR_BASIC, buf);
      count++;
    }
  }
  sendTo("\n\rTotal visible players: %d\n\r", count);
}
