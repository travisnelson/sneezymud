
<?
$page = $_REQUEST['page'];
$which = $_REQUEST['which'];

function runQuery($query, $database) {
         $DBNAME = $database;
         $DBLOGIN = "www";
         $DBPASS = "";
         $DBSERVER = "localhost";

//	$conn = pg_connect("dbname=".$DBNAME." host=".$DBSERVER." user=".$DBLOGIN." password=");

//	$connstr = "dbname=".$DBNAME." user=".$DBLOGIN;
//	echo $connstr;
//      $conn = pg_connect($connstr);
//	$result = pg_exec($conn, $query);

    $link = mysql_connect($DBSERVER, $DBLOGIN, $DBPASS)
	   or die("Couldn't connect to mysql - error 1");
	mysql_select_db($DBNAME)
       or die("Couldn't connect to database - error 2");
   	$result = mysql_query ($query)
       or die("Couldn't query failed - error 3");

    mysql_close($link);
    return $result;
}

if (!isset($page)) $page = "home";

if (file_exists("pages/".$page.".php")) {
	$fd = fopen ("pages/".$page.".php", "r");
	$extra = fgets($fd, 4096);
   	$title = rtrim(fgets($fd, 4096));
	$breadcrumb = rtrim(fgets($fd, 4096));
   	$section = rtrim(fgets($fd, 4096));
	fclose($fd);
} else {
	$title = "404 Page Not Found";
	$breadcrumb = "404 page not found";
	$section = "none";
}

//echo "dash debug: $section == \"".$section."\".";

$HOME_JPG = "images/navbuttons/home.jpg";
$NEWS_JPG = "images/navbuttons/news.jpg";
$PLAY_JPG = "images/navbuttons/players.jpg";
$RESO_JPG = "images/navbuttons/resources.jpg";
$HIST_JPG = "images/navbuttons/histories.jpg";

if ($section == "home")      $HOME_JPG = "images/navbuttons/homeo.jpg";
if ($section == "news")      $NEWS_JPG = "images/navbuttons/newso.jpg";
if ($section == "players")   $PLAY_JPG = "images/navbuttons/playerso.jpg";
if ($section == "resources") $RESO_JPG = "images/navbuttons/resourceso.jpg";
if ($section == "histories") $HIST_JPG = "images/navbuttons/historieso.jpg";

$HOME2_JPG = "images/navbuttons/homeo.jpg";
$NEWS2_JPG = "images/navbuttons/newso.jpg";
$PLAY2_JPG = "images/navbuttons/playerso.jpg";
$RESO2_JPG = "images/navbuttons/resourceso.jpg";
$HIST2_JPG = "images/navbuttons/historieso.jpg";

?>

<html>

<head>
<meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<title>SneezyMUD 5.2 : <?=$title?></title>


<script language="JavaScript">
<!--
// this is code for the image rollovers - autogenerated by dreamweaver yay - dash
function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}

function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_swapImage() { //v3.0
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}
// end img rollover code

//-->
</script>
<link rel="Stylesheet" href="sneezy.css" type="text/css" media="screen">

</head>

<body>

<div align="center">
  <center>
  <table border="0" cellpadding="0" cellspacing="0" width="700">
    <tr>
      <td>
        <div align="center">
          <table border="0" cellpadding="0" width="700" height="209">
            <tr>
              <td height="101">
                <div align="center">
                  <table border="0" cellpadding="0" cellspacing="0" width="100%">
                    <tr>
                      <td width="50%" valign="bottom"><div class="bc"></div></td>
                      <td width="50%"><img border="0" src="sneezytitle.jpg" align="right" width="417" height="100"></td>
                    </tr>
		    <tr>
                      <td colspan="2" width="100%" valign="bottom"><div class="bc">sneezyMUD : <?=$breadcrumb?></div></td>
                                          </tr>

                  </table>
                </div>
              </td>
            </tr>
            <tr>
              <td height="26">
                <div align="center">
                  <table border="1" cellpadding="1" cellspacing="0" width="100%" bordercolorlight="#808080" bordercolordark="#808080" bgcolor="#808080">
                    <tr>
                      <td width="20%">
                        <div align="center">
                          <table border="1" cellpadding="0" cellspacing="0" width="100%" bgcolor="#000000" bordercolorlight="#C0C0C0" bordercolordark="#C0C0C0">
                            <tr>
                              <td width="100%">
                                <p align="center">
		<a href="index.php?page=home" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('home','','<?=$HOME2_JPG?>',1)">
		<img name="home" border="0" src="<?=$HOME_JPG?>" width="69" height="25"></a></p>
                              </td>
                            </tr>
                          </table>
                        </div>
                      </td>
                      <td width="20%">
                        <div align="center">
                          <table border="1" cellpadding="0" cellspacing="0" width="100%" bgcolor="#000000" bordercolorlight="#C0C0C0" bordercolordark="#C0C0C0">
                            <tr>
                              <td width="100%">
                                <p align="center"><a href="index.php?page=news" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('news','','images/navbuttons/newso.jpg',1)"><img name="news" border="0" src="<?=$NEWS_JPG?>" width="64" height="25"></a></p>
                              </td>
                            </tr>
                          </table>
                        </div>
                      </td>
                      <td width="20%">
                        <div align="center">
                          <table border="1" cellpadding="0" cellspacing="0" width="100%" bgcolor="#000000" bordercolorlight="#C0C0C0" bordercolordark="#C0C0C0">
                            <tr>
                              <td width="100%">
                                <p align="center"><a href="index.php?page=players" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('players','','images/navbuttons/playerso.jpg',1)"><img name="players" border="0" src="<?=$PLAY_JPG?>" width="90" height="25"></a></p>
                              </td>
                            </tr>
                          </table>
                        </div>
                      </td>
                      <td width="20%">
                        <div align="center">
                          <table border="1" cellpadding="0" cellspacing="0" width="100%" bgcolor="#000000" bordercolorlight="#C0C0C0" bordercolordark="#C0C0C0">
                            <tr>
                              <td width="100%" align="center"><a href="index.php?page=resources" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('resources','','images/navbuttons/resourceso.jpg',1)"><img name="resources" border="0" src="<?=$RESO_JPG?>" width="119" height="25"></a></td>
                            </tr>
                          </table>
                        </div>
                      </td>                      
                      <td width="20%">
                        <div align="center">
                          <table border="1" cellpadding="0" cellspacing="0" width="100%" bgcolor="#000000" bordercolorlight="#C0C0C0" bordercolordark="#C0C0C0">
                            <tr>
                              <td width="100%">
                                <p align="center"><a href="index.php?page=histories" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('histories','','images/navbuttons/historieso.jpg',1)"><img name="histories" border="0" src="<?=$HIST_JPG?>" width="99" height="25"></a></p>
                              </td>
                            </tr>
                          </table>
                        </div>
                      </td>
                    </tr>
                  </table>
                </div>
              </td>
            </tr>
  </center>
            <tr>
              <td height="31">
                <div align="center">
                  <table border="2" cellpadding="0" cellspacing="0" width="100%" bordercolorlight="#808080" bordercolordark="#808080">
                    <tr>
                      <td width="100%">
                        <div align="center">
                          <table border="1" cellpadding="2" cellspacing="0" width="100%" bordercolorlight="#C0C0C0" bordercolordark="#C0C0C0">
                            <tr>
                              <td width="100%">
                                <div align="center">
                                  <table border="0" cellpadding="0" cellspacing="2" width="687" bordercolorlight="#000000" bordercolordark="#000000">
                                    <tr>
                                      <td width="559" valign="top"><br>
<?
if (file_exists("pages/".$page.".php")) {
	include("pages/".$page.".php");
} else if (file_exists("pages/404.php")) {
	include("pages/404.php");
} 
?>
                                       </td>
                                      <td width="1" height="90%" bgcolor="#808080">
                                        &nbsp;</td>
                                      <td width="115" valign="top">
<?
if (file_exists("pages/navbar.php")) {
	include("pages/navbar.php");
}
?>
                                        </td>
                                    </tr>
                                  </table>
                                </div>
                              </td>
                            </tr>
                          </table>
                        </div>
                      </td>
                    </tr>
                  </table>
                </div>
              </td>
            </tr>
            <tr>
              <td height="20">
                <div align="center">
                  <table border="2" cellpadding="0" cellspacing="0" width="100%" bordercolorlight="#808080" bordercolordark="#808080">
                    <tr>
                      <td width="100%">
                        <div align="center">
                          <table border="1" cellpadding="0" cellspacing="0" width="100%" bordercolorlight="#C0C0C0" bordercolordark="#C0C0C0">
                            <tr>
                              <td width="100%">
                                <div align="center">
                                  <table border="0" cellspacing="0" width="100%">
                                    <tr>
                                      <td width="50%"><p class="footer">All Pages (C) SneezyMUD Development Team, 2005</p></td>
                                    <td width="50%">
                                      <p align="right" class="footer"><em><a href="mailto:webmaster@sneezy.saw.net">webmaster@sneezy.saw.net</a></em></td>
                                  </tr>
                                </table>
                              </div>
                            </td>
                          </tr>
                        </table>
                      </div>
                    </td>
                  </tr>
                </table>
              </div>
              </td>
            </tr>
  <center>
            <tr>
              <td height="21"></td>
            </tr>
          </table>
        </div>
      </center>
      </td>
    </tr>
  </table>
</div>
<div><center><b><p>
To encourage others to play SneezyMUD, please visit the following links and vote for us (you can vote once per day on both sites):<p>
<a href="http://www.mudmagic.com/rate.php?id=1504">mudmagic.com : vote for SneezyMUD</a><p>
<a href="http://www.mudconnector.com/mud_search.html">mudconnector.com : search for SneezyMUD, then vote for it, attempt to connect to it, and click through to the homepage</a>
</body>

</html>
