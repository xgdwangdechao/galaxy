if "%CONFIGURATION%"=="Debug" (
	OpenCppCoverage --sources c:\projects\galaxy --modules c:\projects\galaxy --export_type=cobertura:cobertura.xml -- C:\projects\galaxy\tests\bin\%CONFIGURATION%\%PLATFORM%\tests.exe & codecov --root .. -f cobertura.xml -t %CODECOV_TOKEN%
)