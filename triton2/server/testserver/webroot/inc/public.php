<?php

//============================================================
class RForm{
   public static $action;
   //------------------------------------------------------------
   public static function isView(){
      return ('update' != self::$action && 'insert' != self::$action);
   }
   //------------------------------------------------------------
   public static function isInsert(){
      return ('insert' == self::$action);
   }
   //------------------------------------------------------------
   public static function isUpdate(){
      return ('update' == self::$action) || ('insert' == self::$action);
   }
   //------------------------------------------------------------
   public static function redirect($page){
      print("<HTML><BODY onload=\"window.location='$page'\"></BODY></HTML>");
      return;
   }
};

//============================================================
class RHtml{
   //------------------------------------------------------------
   public static function formatSelect($items, $current){
      foreach($items as $value => $label){
         if($current == $value){
            return $label;
         }
      }
      return null;
   }
   //------------------------------------------------------------
   public static function buildSelect($name, $items, $current){
      $exists = (strlen($current) > 0);
      $html = "<SELECT name='$name'>";
      //$html .= "<OPTION value=''$selected>--- 请选择 ---</OPTION>";
      foreach($items as $value => $label){
         $selected = '';
         if($exists && $current == $value){
            $selected = ' selected';
         }
         $html .= "<OPTION value='$value'$selected>$label</OPTION>";
      }
      $html .= "</SELECT>";
      return $html;
   }
};

//============================================================
class FLoginServerInfo{
   var $id;
   var $host;
   var $port;
   var $type = '';
   var $statusType = '';
   var $status;
   //------------------------------------------------------------
   var $m_typeItems;
   var $m_statusTypeItems;
   var $m_statusItems;
   //------------------------------------------------------------
   function FLoginServerInfo(){
      $this->m_typeItems = array('0'=>'未使用', '1'=>'正式', '2'=>'新服');
      $this->m_statusTypeItems = array('0'=>'系统', '1'=>'定制');
      $this->m_statusItems = array('0'=>'未知', '1'=>'维护', '2'=>'空闲', '3'=>'忙碌', '4'=>'繁忙', '5'=>'火爆');
   }
   //------------------------------------------------------------
   function buildType($name){
      if(0 == strlen($name)){
         $name = 'pv_'.$this->id.'_type';
      }
      if(RForm::isUpdate()){
         return RHtml::buildSelect($name, $this->m_typeItems, $this->type);
      }
      return RHtml::formatSelect($this->m_typeItems, $this->type);
   }
   //------------------------------------------------------------
   function buildStatusType($name){
      if(0 == strlen($name)){
         $name = 'pv_'.$this->id.'_status_type';
      }
      if(RForm::isUpdate()){
         return RHtml::buildSelect($name, $this->m_statusTypeItems, $this->statusType);
      }
      return RHtml::formatSelect($this->m_statusTypeItems, $this->statusType);
   }
   //------------------------------------------------------------
   function buildStatus($name){
      if(0 == strlen($name)){
         $name = 'pv_'.$this->id.'_status';
      }
      if(RForm::isUpdate()){
         return RHtml::buildSelect($name, $this->m_statusItems, $this->status);
      }
      return RHtml::formatSelect($this->m_statusItems, $this->status);
   }
};

//============================================================
class FLoginConfiguration{
   var $m_fileName= 'D:/dev-xyj/source/develop/globalserver/config/data/login.xml';
   var $m_document = null;
   var $loginsConfig = array();
   //------------------------------------------------------------
   // <T>读取服务器信息。</T>
   function readLoginConfig($xconfig){
      // 读取信息
      $loginInfo = new FLoginServerInfo();
      $loginInfo->id = $xconfig->getAttribute('id');
      $loginInfo->host = $xconfig->getAttribute('host');
      $loginInfo->port = $xconfig->getAttribute('port');
      $loginInfo->type = $xconfig->getAttribute('type');
      $loginInfo->statusType = $xconfig->getAttribute('status_type');
      $loginInfo->status = $xconfig->getAttribute('status');
      $this->loginsConfig[$loginInfo->id] = $loginInfo;
   }
   //------------------------------------------------------------
   // <T>读取服务器列表信息。</T>
   function readLoginsConfig($xconfig){
      foreach($xconfig->childNodes as $xnode){
         if('LoginServer' == $xnode->nodeName){
            $this->readLoginConfig($xnode);
         }
      }
   }
   //------------------------------------------------------------
   // <T>读取服务器信息。</T>
   function loadFile(){
      $this->m_document = new DOMDocument();
      $this->m_document->load($this->m_fileName);
      $xconfig = $this->m_document->documentElement;
      // 读取信息
      foreach($xconfig->childNodes as $xnode){
         if('LoginServers' == $xnode->nodeName){
            $this->readLoginsConfig($xnode);
         }
      }
   }
   //------------------------------------------------------------
   // <T>读取服务器信息。</T>
   function saveFile(){
      $xdoc = new DOMDocument();
      $xconfig = $xdoc->createElement('Configuration');
      $xlogins = $xdoc->createElement('LoginServers');
      foreach($this->loginsConfig as $loginConfig){
         if(0 == strlen($loginConfig->id)){
            continue;
         }
         $xlogin = $xdoc->createElement('LoginServer');
         // id
         $aid = $xdoc->createAttribute('id');
         $aid->value = $loginConfig->id;
         $xlogin->setAttributeNode($aid);
         // host
         $ahost = $xdoc->createAttribute('host');
         $ahost->value = $loginConfig->host;
         $xlogin->setAttributeNode($ahost);
         // port
         $aport = $xdoc->createAttribute('port');
         $aport->value = $loginConfig->port;
         $xlogin->setAttributeNode($aport);
         // type
         $atype = $xdoc->createAttribute('type');
         $atype->value = $loginConfig->type;
         $xlogin->setAttributeNode($atype);
         // status_type
         $astatusType = $xdoc->createAttribute('status_type');
         $astatusType->value = $loginConfig->statusType;
         $xlogin->setAttributeNode($astatusType);
         // status
         $astatus = $xdoc->createAttribute('status');
         $astatus->value = $loginConfig->status;
         $xlogin->setAttributeNode($astatus);
         // Append child
         $xlogins->appendChild($xlogin);
      }
      $xconfig->appendChild($xlogins);
      $xdoc->appendChild($xconfig);
      // 保存
      $xdoc->formatOutput = true;
      $xdoc->save($this->m_fileName);
   }
};

?>
