<?php
require('inc/public.php');

RForm::$action = $_REQUEST['do'];
header('Content-Type:text/html;charset=utf-8');

$xconfig = new FLoginConfiguration();
$xconfig->loadFile();

if('save' == RForm::$action){
   // 获取所有内容
   foreach($xconfig->loginsConfig as $loginConfig){
      $loginConfig->type = $_REQUEST["pv_".$loginConfig->id.'_type'];
      $loginConfig->statusType = $_REQUEST["pv_".$loginConfig->id.'_status_type'];
      $loginConfig->status = $_REQUEST["pv_".$loginConfig->id.'_status'];
   }
   // 保存文件
   $xconfig->saveFile();
   return RForm::redirect("list.php");
}else if('delete' == RForm::$action){
   $id = $_REQUEST['id'];
   $xconfig->loginsConfig[$id] = null;
   // 保存文件
   $xconfig->saveFile();
   return RForm::redirect("list.php");
}

?>
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<LINK type='text/css' rel='stylesheet' href='inc/public.css'>
</HEAD>
<SCRIPT>
function doSave(){
   document.body.all['do'].value = 'save';
   fmMain.submit();
}
</SCRIPT>
<BODY>
<FORM name='fmMain' method='post'>
<INPUT type='hidden' name='do'>
<BR>
<H1 align='center'>登录服务器列表</H1>
<TABLE width='100%' border='0'>
<TR>
   <TD></TD>
   <?if(RForm::isView()){?><TD width='60' align='center'><A href='insert.php'>新 建</A></TD><?}?>
   <?if(RForm::isView()){?><TD width='60' align='center'><A href='list.php?do=update'>更 新</A></TD><?}?>
   <?if(RForm::isView()){?><TD width='60' align='center'><A href='list.php'>刷 新</A></TD><?}?>
   <?if(RForm::isView()){?><TD width='60' align='center'><A href='list.php?do=process'>执 行</A></TD><?}?>
   <?if(RForm::isUpdate()){?><TD width='60' align='center'><A href='javascript:doSave()'>保 存</A></TD><?}?>
   <?if(RForm::isUpdate()){?><TD width='60' align='center'><A href='list.php'>返 回</A></TD><?}?>
</TR>
</TABLE>
<HR>
<TABLE width='100%' border='0' cellspacing='1' cellpadding='4' layout='box' bgcolor='#999999'>
<TR bgcolor='#CCCCCC' style='font:bold;color:darkblue' align='center' height='30'>
   <TD width='80'>编号</TD>
   <TD width='120'>主机</TD>
   <TD width='80'>端口</TD>
   <TD width='140'>类型</TD>
   <TD width='140'>状态类型</TD>
   <TD width='140'>状态</TD>
   <TD>在线人数</TD>
   <TD width='80'>命令</TD>
</TR>
<?foreach($xconfig->loginsConfig as $loginConfig){?>
<TR bgcolor='#FFFFFF' height='30'>
   <TD align='center'><?print($loginConfig->id)?></TD>
   <TD align='center'><?print($loginConfig->host)?></TD>
   <TD align='center'><?print($loginConfig->port)?></TD>
   <TD><?print($loginConfig->buildType(''))?></TD>
   <TD><?print($loginConfig->buildStatusType(''))?></TD>
   <TD><?print($loginConfig->buildStatus(''))?></TD>
   <TD><?print($loginConfig->online)?></TD>
   <TD align='center'>
      <?if(RForm::isView()){?>
      <A href='list.php?do=delete&id=<?print($loginConfig->id)?>'>删 除</A>
      <?}?>
   </TD>
</TR>
<?}?>
</TABLE>
</FORM>
</BODY>
</HTML>
