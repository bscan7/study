@echo off  
set TMPP=*.~* *.aps *.bak *.dcu *.ddp *.old *.plg *.user *.idb *.ncp *.obj *.pch *.sbr *.tmp *.pdb *.bsc *.ilk *.res *.ncb *.opt *.suo *.dep *.log .git
echo ----------------------------------------------------  
echo By Bscan
echo 即将删除目录：%cd% 中，所有以下类型的文件:  
echo  %TMPP%  
echo 它们是 Visual C++ and Visual Studio 临时文件  
echo ----------------------------------------------------  
pause  
del /F /S /Q    %TMPP%
echo ======%cd% 中的所有临时文件清理完毕！  
pause  