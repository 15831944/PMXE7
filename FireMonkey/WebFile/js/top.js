 function a(){
   //var winHeight = 0;
if (window.innerWidth)//for ie
{
   winHeight = window.innerHeight;
}
else ((document.body) && (document.body.clientHeight))
{
   winHeight = document.body.clientHeight;
}

//ͨ������Document�ڲ���body���м�⣬��ȡ���ڴ�С
if (document.documentElement && document.documentElement.clientHeight)
{
   winHeight = document.documentElement.clientHeight;
}
//return winHeight;
	//var d= window.screen.availHeight;
	//alert(winHeight);
	box.style.height=winHeight-88;
	}

