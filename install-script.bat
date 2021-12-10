md Release
copy Dependencies\assimp\lib\assimpd.dll Release\
Xcopy /s /i rendering_engine\models Release\models
Xcopy /s /i rendering_engine\shader_source Release\shader_source

md Debug
copy Dependencies\assimp\lib\assimpd.dll Debug\
Xcopy /s /i rendering_engine\models Debug\models
Xcopy /s /i rendering_engine\shader_source Debug\shader_source