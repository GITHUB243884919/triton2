<?php
require('inc/public.php');
// 设置命令
$action = $_REQUEST['do'];
RForm::$action = $action;
if(0 == strlen(RForm::$action)){
   RForm::$action = 'insert';
}
$loginConfig = new FLoginServerInfo();
// 保存操作
$check = true;
$checkId = true;
$checkHost = true;
$checkPort = true;
$checkType = true;
$checkStatusType = true;
$checkStatus = true;
if('save' == $action){
   $loginConfig->id = $_REQUEST['pv_id'];
   $loginConfig->host = $_REQUEST['pv_host'];
   $loginConfig->port = $_REQUEST['pv_port'];
   $loginConfig->type = $_REQUEST['pv_type'];
   $loginConfig->statusType = $_REQUEST['pv_status_type'];
   $loginConfig->status = $_REQUEST['pv_status'];
   // 检查数据
   if(0 == strlen($loginConfig->id)){
      $check = false;
      $checkId = false;
   }
   if(0 == strlen($loginConfig->host)){
      $check = false;
      $checkHost = false;
   }
   if(0 == strlen($loginConfig->port)){
      $check = false;
      $checkPort = false;
   }
   if(0 == strlen($loginConfig->type)){
      $check = false;
      $checkType = false;
   }
   if(0 == strlen($loginConfig->statusType)){
      $check = false;
      $checkStatusType = false;
   }
   if(0 == strlen($loginConfig->status)){
      $check = false;
      $checkStatus = false;
   }
   // 结果正确
   if($check){
      $xconfig = new FLoginConfiguration();
      $xconfig->loadFile();
      $xconfig->loginsConfig[$loginConfig->id] = $loginConfig;
      $xconfig->saveFile();
      return RForm::redirect("list.php");
   }
   RForm::$action = 'insert';
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
<STYLE>
.tableHead{
   font:bold;
   color:darkblue;
}
.tableInfo{
   background-color:#FFFFFF
}
.tableNote{
   color:green;
   background-color:#FFFFFF
}
</STYLE>
<BODY>
<FORM name='fmMain' method='post'>
<INPUT type='hidden' name='do'>
<BR>
<H1 align='center'>新建服务器</H1>

<TABLE width='100%' border='0'>
<TR>
   <TD></TD>
   <TD width='60' align='center'><A href='javascript:doSave()'>保 存</A></TD>
   <TD width='60' align='center'><A href='list.php'>返 回</A></TD>
</TR>
</TABLE>

<TABLE width='100%' border='0' cellspacing='1' cellpadding='4' layout='box' bgcolor='#999999'>
<TR bgcolor='#CCCCCC' height='30'>
   <TD width='140' class='tableHead' style='color:red'>编号</TD>
   <TD class='tableInfo'><INPUT name='pv_id' style='width:100%' value='<?print($loginConfig->id)?>'></TD>
   <TD width='400' class='tableNote'>
      <? if($checkId){ ?>登录服务器的编号。（必填项）<? }else{ ?><FONT color='red'>输入非法。</FONT><? } ?>
   </TD>
</TR>
<TR bgcolor='#CCCCCC' height='30'>
   <TD class='tableHead' style='color:red'>主机</TD>
   <TD class='tableInfo'><INPUT name='pv_host' style='width:100%' value='<?print($loginConfig->host)?>'></TD>
   <TD class='tableNote'>
      <? if($checkHost){ ?>登录服务器的主机名称。（必填项）<? }else{ ?><FONT color='red'>输入非法。</FONT><? } ?>
   </TD>
</TR>
<TR bgcolor='#CCCCCC' height='30'>
   <TD class='tableHead' style='color:red'>端口</TD>
   <TD class='tableInfo'><INPUT name='pv_port' style='width:100%' value='<?print($loginConfig->port)?>'></TD>
   <TD class='tableNote'>
      <? if($checkPort){ ?>登录服务器的端口。（必填项）<? }else{ ?><FONT color='red'>输入非法。</FONT><? } ?>
   </TD>
</TR>
<TR bgcolor='#CCCCCC' height='30'>
   <TD class='tableHead'>类型</TD>
   <TD class='tableInfo'><?print($loginConfig->buildType('pv_type'))?></TD>
   <TD class='tableNote'>
      <? if($checkType){ ?>登录服务器的类型。<? }else{ ?><FONT color='red'>请选择内容。</FONT><? } ?>
   </TD>
</TR>
<TR bgcolor='#CCCCCC' height='30'>
   <TD class='tableHead'>状态类型</TD>
   <TD class='tableInfo'><?print($loginConfig->buildStatusType('pv_status_type'))?></TD>
   <TD class='tableNote'>
      <? if($checkStatusType){ ?>登录服务器的状态类型。<? }else{ ?><FONT color='red'>请选择内容。</FONT><? } ?>
   </TD>
</TR>
<TR bgcolor='#CCCCCC' height='30'>
   <TD class='tableHead'>状态</TD>
   <TD class='tableInfo'><?print($loginConfig->buildStatus('pv_status'))?></TD>
   <TD class='tableNote'>
      <? if($checkStatus){ ?>登录服务器的状态。<? }else{ ?><FONT color='red'>请选择内容。</FONT><? } ?>
   </TD>
</TR>
</TABLE>
</HTML>
