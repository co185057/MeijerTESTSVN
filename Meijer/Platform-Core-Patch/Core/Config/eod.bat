echo End Of Day processing

rem --Delete signature files--
pushd C:\temp
for %%f in (SCPIC*.PNG) do del /q %%f
popd
