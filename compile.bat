set x=%cd%
set lst="\=/",":="
for %%i in (%lst%) do call set x=%%x:%%~i%%
echo(%x%
docker container create --rm --name gcc-compiler -it gcc-compiler-image
docker container cp gcc-compiler:/out .
pause
