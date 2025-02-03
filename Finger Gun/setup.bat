@echo seting up Project %CD% and MVSC compiler (vcvars64)....

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
rem devenv build/scratch.exe
code .

