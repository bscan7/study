#include <Constants.au3>
#include <Array.au3>
#include <GuiTab.au3>


#CS Local $aArray[5] = [0, 1, 2, 3, 4]
###
### _ArrayDisplay($aArray, "Original")
### _ArrayDelete($aArray, 2)
### _ArrayDisplay($aArray, "Element 2 deleted")
###
### Exit
 #CE
;
; AutoIt Version: 3.0
; Language:       English
; Platform:       Win9x/NT
; Author:         Jonathan Bennett (jon at autoitscript dot com)
;
; Script Function:
;   Demo of using multiple lines in a message box
;

; Use the @CRLF macro to create a newline in a MsgBox - it is similar to the \n in v2.64
$hWnd = WinWait("Memory Operation")

; Get handle to Tab control
$hTab = ControlGetHandle($hWnd, "", "[NAME:TabControl1]")
If @error Then _Exitonerror("Failed to get handle to TAB control.")
ConsoleWrite(@CRLF & _GUICtrlTab_GetCurFocus($hTab))

Func FindIt(ByRef $sString, ByRef $iOffset) ; Swap the contents of two variables.
   ;Local $sString = "00 00 00 00 01 00 00 00 FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 80 3F"

   While (_GUICtrlTab_GetCurFocus($hTab) <> 3)
   _GUICtrlTab_SetCurFocus($hTab, 3)
   _GUICtrlTab_SetCurSel($hTab, 3)
   WEnd


   ;输入搜索串
   ConsoleWrite(@CRLF & "输入搜索串")
   ControlSetText($hWnd, "", "[NAME:TextBox5]", $sString)
   ;清空结果框
   ConsoleWrite(@CRLF & "清空结果框")
   ControlSetText($hWnd, "", "[NAME:TextBox7]", "")

   ConsoleWrite(@CRLF & "ControlGetText")
   While StringLen ( ControlGetText($hWnd, "", "[NAME:TextBox7]") ) > 0
	   Sleep(100)
   WEnd

   ConsoleWrite(@CRLF & "点击Find按钮")
   ;点击Find按钮
   ControlClick($hWnd, "", "[NAME:Button10]")

   ConsoleWrite(@CRLF & "读取结果框")
   $sString = ""
   While StringLen ( $sString ) <= 0
	   Sleep(100)
	   $sString = ControlGetText($hWnd, "", "[NAME:TextBox7]")
   WEnd

   ;Local $sText = ControlGetText(WinWait("Memory Operation"), "", "[NAME:TextBox5]")

   Local $aArray_Base[1][2] = [["地址", "内容"]]
   ;_ArrayDisplay($aArray_Base, "2D - 2D Array")

   ConsoleWrite(@CRLF & "结果框")
   If StringInStr ($sString ,"Not Found!")  == 0 Then ;Found(17)!
	  ;MsgBox($MB_SYSTEMMODAL, "AutoIt Example", $sString & @CRLF & "This is line 2" & @CRLF & "This is line 3")
	  Local $aArray = StringSplit($sString, " 0x", $STR_ENTIRESPLIT) ; Pass the variable to StringSplit and using the delimiter "\n".
	   ; Note that flag parameter is set to $STR_ENTIRESPLIT (1) as it would split at \ or n otherwise.
	   #cs
		   The array returned will contain the following values:
		   $aArray[1] = "This"
		   $aArray[2] = "line"
		   ...
		   $aArray[4] = "C-style breaks."
	   #ce

	   _ArrayDelete($aArray,1)
	   $aArray[0] = $aArray[0] -1

	   For $i = 1 To $aArray[0] ; Loop through the array returned by StringSplit to display the individual values.
		  Local $sHex =  StringSplit($aArray[$i], " ", $STR_ENTIRESPLIT)[1]
		  Local $iDec = Dec($sHex, $NUMBER_32BIT ) +$iOffset

		  ConsoleWrite(@CRLF & @CRLF & "$aArray[" & $i & "] - " & $sHex & " - " & $iDec )
		   ;MsgBox($MB_SYSTEMMODAL, "", "$aArray[" & $i & "] - " & $aArray[$i])

		 ConsoleWrite(@CRLF & "输入地址")
		 ControlSetText($hWnd, "", "[NAME:tbAddr]", Hex($iDec))
		 ConsoleWrite(@CRLF & "输入Len")
		 ControlSetText($hWnd, "", "[NAME:tbSize]", "4")
		 ConsoleWrite(@CRLF & "清空结果")
		 ControlSetText($hWnd, "", "[NAME:tbData]", "")
		 ConsoleWrite(@CRLF & "点击Read按钮")
		 ;点击Find按钮
		 ControlClick($hWnd, "", "[NAME:Button1]")

		 ConsoleWrite(@CRLF & "读取内存：")
		 $sString = ""
		 While StringLen ( $sString ) <= 0
			 Sleep(100)
			 $sString = ControlGetText($hWnd, "", "[NAME:tbData]")
		  WEnd

		  If StringInStr ($sString ,"00 00 00 00")  == 0 Then
		   ConsoleWrite( $sString)
		   _ArrayAdd($aArray_Base, Hex($iDec) & "|" & $sString)

		  Else
		   _ArrayDelete($aArray,$i)
		   $i = $i -1
		   $aArray[0] = $aArray[0] -1
		EndIf

		If $i >= $aArray[0] Then
			ExitLoop
		 EndIf

		 ConsoleWrite(@CRLF & "读取内存...$i = " & $i  & " -> " &$aArray[0])

	   Next

   EndIf
   ConsoleWrite(@CRLF )
   ;_ArrayDisplay($aArray, "Original")
   ;_ArrayDisplay($aArray_Base, "$aArray_Base")
   _ArrayDelete($aArray_Base,0)
   Sleep(1000)
   Local $iRows = UBound($aArray_Base, $UBOUND_ROWS) ; Total number of rows. In this example it will be 10.
   ConsoleWrite(@CRLF & $i & "======================================" & $iRows)
    For $i = 0 To $iRows - 1

		 	  ConsoleWrite(@CRLF & "输入地址")
			   ConsoleWrite(@CRLF & $i & "======================================" & $iRows)
			   ControlSetText($hWnd, "", "[NAME:tbAddr]", Hex(Dec($aArray_Base[$i][0], $NUMBER_32BIT )))
			   ConsoleWrite(@CRLF & "输入Len")
			   ControlSetText($hWnd, "", "[NAME:tbSize]", "4")
			   ConsoleWrite(@CRLF & "清空结果")
			   ControlSetText($hWnd, "", "[NAME:tbData]", "")
			   ConsoleWrite(@CRLF & "点击Read按钮")
			   ;点击Find按钮
			   ControlClick($hWnd, "", "[NAME:Button1]")

			   ConsoleWrite(@CRLF & "读取内存：")
			   $sString = ""
			   While StringLen ( $sString ) <= 0
				   Sleep(100)
				   $sString = ControlGetText($hWnd, "", "[NAME:tbData]")
				WEnd

				If StringInStr ($sString ,$aArray_Base[$i][1])  == 0 Then
				 ConsoleWrite( $sString)
				 ;_ArrayAdd($aArray_Base, Hex($iDec) & "|" & $sString)

				Else
				 _ArrayDelete($aArray_Base,$i)

				 ConsoleWrite( "_ArrayDelete($aArray_Base,"&$i&")")
				 $i = $i -1
				 $iRows = $iRows -1
			  EndIf

			If $i >= ($iRows -1) Then
			   ExitLoop
			EndIf


    Next
    Return $aArray_Base
 EndFunc

Local $iOffset = -20
Local $sString = "00 00 00 00 01 00 00 00 FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 80 3F"

$aArray_Base1 = FindIt($sString, $iOffset) ;实际坐标地址 = 此地址再 -132
;_ArrayDisplay($aArray_Base1, "$aArray_Base")

;矩阵地址:
$sString = "00 00 80 3f 00 00 80 3f 00 00 80 3f 00 00 80 3f 00 00 00 00 00 00 00 00 00 00 80 3f 00 00 00 00 00 00 00 00 00 00 80 3f 00 00 80 3f"
$iOffset = 200
;$aArray_Base1 = FindIt($sString, $iOffset) ;实际矩阵地址 = 此地址再 + 8
_ArrayDisplay($aArray_Base1, "$aArray_Base")


#CS If $sString > 0 Then
###     MsgBox($MB_SYSTEMMODAL, "", "Value is positive.")
### ElseIf $sString < 0 Then
###     MsgBox($MB_SYSTEMMODAL, "", "Value is negative.")
### Else
###     If StringIsXDigit($sString) Then
###         MsgBox($MB_SYSTEMMODAL, "", "Value might be hexadecimal!")
###     Else
###         MsgBox($MB_SYSTEMMODAL, "", "Value is a string.")
###     EndIf
### EndIf
###
 #CE
