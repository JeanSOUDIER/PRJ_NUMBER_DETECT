
call "setup_msvc.bat"


call  "\\DESKTOP-JEAN\D$\Matlab R2020b\bin\win64\checkMATLABRootForDriveMap.exe" "\\DESKTOP-JEAN\D$\Matlab R2020b"  > mlEnv.txt
for /f %%a in (mlEnv.txt) do set "%%a"\n
cd .

chcp 1252

if "%1"=="" (nmake MATLAB_ROOT=%MATLAB_ROOT% ALT_MATLAB_ROOT=%ALT_MATLAB_ROOT% MATLAB_BIN=%MATLAB_BIN% ALT_MATLAB_BIN=%ALT_MATLAB_BIN%  -f untitled.mk all) else (nmake MATLAB_ROOT=%MATLAB_ROOT% ALT_MATLAB_ROOT=%ALT_MATLAB_ROOT% MATLAB_BIN=%MATLAB_BIN% ALT_MATLAB_BIN=%ALT_MATLAB_BIN%  -f untitled.mk %1)
@if errorlevel 1 goto error_exit

exit 0

:error_exit
echo The make command returned an error of %errorlevel%
exit 1