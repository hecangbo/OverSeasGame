rmdir /s/q assets
xcopy /s /y ..\..\..\Resources\csb\* .\assets\csb\
xcopy /s /y ..\..\..\Resources\res\* .\assets\res\
cocos luacompile -s ..\..\..\Resources\Script -d .\assets\Script -e -k 2dxLua -b XXTEA --disable-compile