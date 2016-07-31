@echo off
@title Red Sun Over Paradise Plugin Compiler

for %%f in (*.sp) do (
	@echo --- COMPILING: %%~nf
	spcomp %%f -i="..\_includes"

	if exist "%%~nf.smx" (
		@echo --- MOVING %%~nf
		
		for %%d in (.) do (
			if exist "..\_compiled\%%~nd" (
				cmd /c move "%%~nf.smx" "..\_compiled\%%~nd\%%~nf.smx"
			) else (
				@echo --- Directory for plugin does not exist, creating...
				mkdir "..\_compiled\%%~nd"
				cmd /c move "%%~nf.smx" "..\_compiled\%%~nd\%%~nf.smx"
			)
		)
		@echo --- OPERATION DONE
	) else (
		@echo --- FILE WAS NOT COMPILED, SKIPPING
		@echo --- Please take time to read the errors it spit out and fix them accordingly
	)

	@echo.
	@echo.
)

pause