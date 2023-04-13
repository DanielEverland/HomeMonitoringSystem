:: https://stackoverflow.com/a/10021590
@echo off
set inputFile=Website.html
set outputFile=Website.generated.h
setLocal EnableDelayedExpansion
echo const char WEBSITE[] = R^"rawliteral(>%outputFile%
for /f "tokens=* delims= " %%a in (%inputfile%) do (
set /a N+=1
echo %%a>>%outputFile%
)
echo )rawliteral^";>>%outputFile%