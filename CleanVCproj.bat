@echo off  
set TMPP=*.~* *.aps *.bak *.dcu *.ddp *.old *.plg *.user *.idb *.ncp *.obj *.pch *.sbr *.tmp *.pdb *.bsc *.ilk *.res *.ncb *.opt *.suo *.dep *.log .git
echo ----------------------------------------------------  
echo By Bscan
echo ����ɾ��Ŀ¼��%cd% �У������������͵��ļ�:  
echo  %TMPP%  
echo ������ Visual C++ and Visual Studio ��ʱ�ļ�  
echo ----------------------------------------------------  
pause  
del /F /S /Q    %TMPP%
echo ======%cd% �е�������ʱ�ļ�������ϣ�  
pause  