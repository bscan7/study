gg.toast('南风qq1120918716群487224731')
gg.alert('增加人物天线和人物变大，请小号娱乐,南风q群487224731')
print('南风qq1120918716群487224731') 
--Game guardian detection

if gg.isVisible(true) then 
  gg.setVisible(false) 
end 
gg.clearResults()
gg.toast('作者南风q1120918716群487224731')
goto START




::START::
menu = gg.choice({'⭕人物加速(落地后开启，不太稳定)','⭕修改分辨率(大厅中开)','⭕人物穿墙(可手动冻结)','⭕人物天线(游戏中开)','⭕人物上色(游戏中开)','⭕除草(落地开)','⭕人物变大(搞笑外挂大头娃娃)','⭕人物天线②'},nil,'❌www.wgh.la 挂海辅助网❌')
if menu == 1 then goto X1 end
if menu == 2 then goto X2 end
if menu == 3 then goto X3 end
if menu == 4 then goto X4 end
if menu == 5 then goto shangse end
if menu == 6 then goto X5 end
if menu == 7 then goto X8 end
if menu == 8 then goto X9 end
if menu == nil then print('Error') end 
goto sdone


::X1::
gg.clearResults()
gg.searchNumber('1;256D;0.60000002384;25,000,000.0', gg.TYPE_FLOAT, false, gg.SIGN_EQUAL, 0, -1)
gg.searchNumber('1', gg.TYPE_FLOAT, false, gg.SIGN_EQUAL, 0, -1)
gg.getResults(100)
gg.editAll('3', gg.TYPE_FLOAT)
gg.toast('第一步已开启，请等待第二步开启！')
gg.clearResults()
gg.clearResults()
gg.searchNumber('0.81915205717;45;0.70710676908;1;0.125',gg.TYPE_FLOAT, false, gg.SIGN_EQUAL, 0, -1)
gg.searchNumber('1', gg.TYPE_FLOAT, false, gg.SIGN_EQUAL, 0, -1)
gg.getResults(100)
gg.editAll('0.5', gg.TYPE_FLOAT)
gg.toast('人物加速开启成功')
goto sdone
 



::X2::
gg.clearResults()
gg.searchNumber('65,793D;1.0F;2,048D;16D', gg.TYPE_FLOAT, false, gg.SIGN_EQUAL, 0, -1)
gg.searchNumber('1', gg.TYPE_FLOAT, false, gg.SIGN_EQUAL, 0, -1)
gg.getResults(100)
gg.editAll('1.7', gg.TYPE_FLOAT)
gg.toast('视角扩大已开启！')
gg.clearResults()
goto sdone

::X3::
gg.clearResults()
gg.searchNumber('30;58::10',gg.TYPE_FLOAT,false, gg.SIGN_EQUAL,0, -1)
gg.searchNumber('30', gg.TYPE_FLOAT,false,gg.SIGN_EQUAL, 0, -1)
gg.getResults(10)
gg.editAll('-30', gg.TYPE_FLOAT)
gg.toast('穿墙已开启请手动冻结')
goto sdone


::X4::
gg.clearResults()
gg.searchNumber('1F;0.61365634203F;0.00034569605F;0.78957355022F',gg.TYPE_FLOAT,false, gg.SIGN_EQUAL,0, -1)
gg.searchNumber('0.61365634203F;0.00034569605F;0.78957355022F', gg.TYPE_FLOAT,false,gg.SIGN_EQUAL, 0, -1)
gg.getResults(100)
gg.editAll('888', gg.TYPE_FLOAT)
gg.toast('人物天线已开启')
goto sdone


::X5::
gg.clearResults()
gg.searchNumber('8;1;14000D;15000D;1D', gg.TYPE_FLOAT, false, gg.SIGN_EQUAL, 0, -1)
gg.searchNumber('8', gg.TYPE_FLOAT, false, gg.SIGN_EQUAL, 0, -1)
gg.getResults(10)
gg.editAll('0', gg.TYPE_FLOAT)
gg.toast('除草已开启！')
gg.clearResults()
goto sdone


::X8::
gg.clearResults()
gg.searchNumber('-256D;1;1;1;604D;1;604D',gg.TYPE_FLOAT,false, gg.SIGN_EQUAL,0, -1)
gg.searchNumber('1', gg.TYPE_FLOAT,false,gg.SIGN_EQUAL, 0, -1)
gg.getResults(10)
gg.editAll('1.2', gg.TYPE_FLOAT)
gg.toast('人物变大开启成功')
goto sdone

::X9::
gg.clearResults()
gg.searchNumber('1;0.61365634203;0.00034569605;0.78957355022',gg.TYPE_FLOAT,false, gg.SIGN_EQUAL,0, -1)
gg.searchNumber('1', gg.TYPE_FLOAT,false,gg.SIGN_EQUAL, 0, -1)
gg.getResults(10)
gg.editAll('666', gg.TYPE_FLOAT)
gg.toast('人物天线开启成功，人机没效果！')
goto sdone



::shangse::
gg.clearResults()
menu = gg.choice({'⭕绿色','⭕蓝色','⭕冰雪色','⭕蓝绿色','❌返回上一层'})
if menu == 1 then goto Y1 end
if menu == 2 then goto Y2 end
if menu == 3 then goto Y3 end
if menu == 4 then goto Y4 end
if menu == 5 then goto START end
if menu == nil then print('            ') end 
if menu == nil then print('                                                                 ') end 
if menu == nil then print('            ') end 
if menu == nil then print('                                                                 ') end 
if menu == nil then print('            ') end 
goto sdone

::Y1::
gg.clearResults()
gg.searchNumber('0.09112548828',gg.TYPE_FLOAT,false, gg.SIGN_EQUAL,0, -1)
gg.searchNumber('0.09112548828', gg.TYPE_FLOAT,false,gg.SIGN_EQUAL, 0, -1)
gg.getResults(5)
gg.editAll('-99', gg.TYPE_FLOAT)
gg.toast('人物绿色已开启')
goto sdone

::Y2::
gg.clearResults()
gg.searchNumber('0.30737304688',gg.TYPE_FLOAT,false, gg.SIGN_EQUAL,0, -1)
gg.searchNumber('0.30737304688', gg.TYPE_FLOAT,false,gg.SIGN_EQUAL, 0, -1)
gg.getResults(5)
gg.editAll('-99', gg.TYPE_FLOAT)
gg.toast('人物蓝色已开启')
goto sdone

::Y3::
gg.clearResults()
gg.searchNumber('0.30737304688;0.09112548828',gg.TYPE_FLOAT,false, gg.SIGN_EQUAL,0, -1)
gg.searchNumber('0.30737304688;0.09112548828', gg.TYPE_FLOAT,false,gg.SIGN_EQUAL, 0, -1)
gg.getResults(10)
gg.editAll('-99', gg.TYPE_FLOAT)
gg.toast('冰雪色已开启')
goto sdone

::Y4::
gg.clearResults()
gg.searchNumber('0.30737304688;0.09112548828',gg.TYPE_FLOAT,false, gg.SIGN_EQUAL,0, -1)
gg.searchNumber('0.30737304688', gg.TYPE_FLOAT,false,gg.SIGN_EQUAL, 0, -1)
gg.getResults(4)
gg.editAll('-99', gg.TYPE_FLOAT)
gg.toast('正在开启')
gg.clearResults()
gg.searchNumber('0.30737304688;0.09112548828',gg.TYPE_FLOAT,false, gg.SIGN_EQUAL,0, -1)
gg.searchNumber('0.09112548828', gg.TYPE_FLOAT,false,gg.SIGN_EQUAL, 0, -1)
gg.getResults(4)
gg.editAll('10', gg.TYPE_FLOAT)
gg.toast('已开启')
goto sdone






::sdone:: 
print('感谢支持挂海辅助网的兄弟')
print('也希望你们多多支持南风T')
gg.toast('南风qq1120918716群487224731')
os.exit()

