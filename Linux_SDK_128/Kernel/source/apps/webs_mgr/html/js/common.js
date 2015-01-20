/*Common functions for all pages
Author: Sajith
Date: 12th Oct 2012
*/

/*var getSystemSettings=function(names,callback)
{
	var command="../cgi-bin/Web_Get_Sys_Cfg.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i];
	}
	callAjax(command,0,callback); //Read Request
	
};
var setSystemSettings=function(names,values,callback)
{
	var command="../cgi-bin/Web_Set_Sys_Cfg.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i]+"="+values[i];
	}
	callAjax(command,1,callback);//Set request
};*/
var getNetworkSettings=function(names,callback)
{
	var command="../cgi-bin/Web_Get_Nw_Cfg.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i];
	}
	callAjax(command,0,callback); //Read Request
	
};
var setNetworkSettings=function(names,values,callback)
{
	var command="../cgi-bin/Web_Set_Nw_Cfg.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i]+"="+values[i];
	}
	callAjax(command,1,callback);//Set request
};
var getWirelessSettings=function(names,callback)
{
	var command="../cgi-bin/Web_Get_Wlan_Cfg.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i];
	}
	callAjax(command,0,callback); //Read Request
	
};
var setWirelessSettings=function(names,values,callback)
{
	var command="../cgi-bin/Web_Set_Wlan_Cfg.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i]+"="+values[i];
	}
	callAjax(command,1,callback);//Set request
};
/*var getServerSettings=function(names,callback)
{
	var command="../cgi-bin/Web_Get_Srv_Cfg.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i];
	}
	callAjax(command,0,callback); //Read Request
	
};
var setServerSettings=function(names,values,callback)
{
	var command="../cgi-bin/Web_Set_Srv_Cfg.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i]+"="+values[i];
	}
	callAjax(command,1,callback);//Set request
};
var getMacList=function(names,callback)
{
	var command="../cgi-bin/Web_Get_Mac_List.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i];
	}
	callAjax(command,0,callback); //Read Request
	
};



var getNetworkStates=function(names,callback)
{
	var command="../cgi-bin/Web_Get_Net_Stats.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i];
	}
	callAjax(command,0,callback); //Read Request
	
};
*/
var setConfig=function(names,values,callback)
{
	var command="../cgi-bin/Web_Set_Mgt_Cfg.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i]+"="+values[i];
	}
	callAjax(command,1,callback);//Set request
};
/*
var getTimeSettings=function(names,callback)
{
	var command="../cgi-bin/Web_Get_TProf_Cfg.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i];
	}
	callAjax(command,0,callback); //Read Request
	
};
var setTimeSettings=function(names,values,callback)
{
	var command="../cgi-bin/Web_Set_TProf_Cfg.cgi?";
	for(var i=0;i<names.length;i++)
	{
		if(i>0)
			command=command+"&";
		command=command+names[i]+"="+values[i];
	}
	callAjax(command,1,callback);//Set request
};*/
var callAjax=function(command,type,callback)
{
//alert("command "+command);
$.ajax({
	type: "GET",
	url: command,
	//contentType: 'application/json; charset=utf-8',
  //  dataType: "json",
	cache: false,
	beforeSend: function (XMLHttpRequest) {  
 
	},
	complete: function(XMLHttpRequest, textStatus) {
	
	},
	success: function(msg, textStatus) {
			//if(type==0)
			//alert("msg : "+msg+" sts"+textStatus);
				output(msg,callback);
			//else
				alert(msg);
	},
	error: function(XMLHttpRequest, err) {
	  alert('Cannot Process the request'); 
	}
	});//End of Ajax	
};


var output=function(response,callback)
{
	var n=response.split("\n");
	values = new Array(n.length);
	for(var i=0;i<n.length;i++)
	{
		var m=n[i].split("=");
		values[i]=m[1];
	}
	if(callback)
	 callback(values);

}

