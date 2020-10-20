call "setup_msvc.bat"

cd .

chcp 1252

if "%1"=="" (nmake  -f sim_model_ver_tp2_V2.mk all) else (nmake  -f sim_model_ver_tp2_V2.mk %1)
@if errorlevel 1 goto error_exit

exit 0

:error_exit
echo The make command returned an error of %errorlevel%
exit 1
