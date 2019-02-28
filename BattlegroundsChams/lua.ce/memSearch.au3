#include <Constants.au3>

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

Local $sString = "00 00 00 00 01 00 00 00 FF FF FF FF 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 80 3F"
;输入搜索串
ControlSetText($hWnd, "", "[NAME:TextBox5]", $sString)
;清空结果框
ControlClick($hWnd, "", "[NAME:Button11]")
ControlClick($hWnd, "", "[NAME:Button11]")
While StringLen ( ControlGetText(WinWait("Memory Operation"), "", "[NAME:TextBox7]") ) > 0
    Sleep(100)
WEnd

;点击Find按钮
ControlClick($hWnd, "", "[NAME:Button10]")

$sString = ""
While StringLen ( $sString ) <= 0
    Sleep(100)
	$sString = ControlGetText(WinWait("Memory Operation"), "", "[NAME:TextBox7]")
WEnd

;Local $sText = ControlGetText(WinWait("Memory Operation"), "", "[NAME:TextBox5]")

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

    For $i = 2 To $aArray[0] ; Loop through the array returned by StringSplit to display the individual values.
	   Local $sHex =  StringSplit($aArray[$i], " ", $STR_ENTIRESPLIT)[1]
	   Local $iDec = Dec($sHex, $NUMBER_64BIT ) - 20

	   ConsoleWrite(@CRLF & "$aArray[" & $i-1 & "] - " & $sHex & " - " & $iDec )
        ;MsgBox($MB_SYSTEMMODAL, "", "$aArray[" & $i & "] - " & $aArray[$i])
    Next

EndIf
ConsoleWrite(@CRLF )

[NAME:tbAddr]
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
