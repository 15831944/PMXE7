	//crt�Ƕ���Ĭ��ID�ģ����û����crt����""����.
	var crt = "";//document.getElementById("rsv");
	
	//crt.className�Ƕ���Ĭ��״̬����ʽ�����û��Ĭ��״̬����Ϊ�������С�
	//crt.className='sysmenu-cake-click';
	
	//returnObj�ǹ��÷�������ȡһ������
	function returnObj(obj){   
		if(typeof obj == 'object'){
			return obj;
		}else
		if(typeof obj == 'string'){
			return document.getElementById(obj);
		}
	}
	
	//����¼�����ʽ�ı亯��
	function over(obj){
		obj = returnObj(obj);
		if(obj == crt){
				obj.className='sysmenu-cake-click';
		}else{
			obj.className='sysmenu-cake-over';
		}
	}
	function out(obj){
		obj = returnObj(obj);
		if(obj == crt){			
			obj.className='sysmenu-cake-click';
		}else{
			obj.className='sysmenu-cake';
		}
	}	
	
	//click1()���ʱ�ı���ʽ�����Ҽ�¼��ǰ����
	function click1(obj){
		obj = returnObj(obj);
		if(crt != ""){
			crt.className='sysmenu-cake';
		}
		crt = obj;
		obj.className='sysmenu-cake-click';
	}
	

