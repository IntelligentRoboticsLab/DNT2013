-- special pathes which can be configured manualy. 
-- If a path is set to 'nil' the default value is used. 
-- for default values check projectconfig.lua

-- default: "../../Framework"
FRAMEWORK_PATH = nil

-- for native platform
-- default: "../../Extern"
EXTERN_PATH_NATIVE = nil
-- webots instalation if available
-- default: os.getenv("WEBOTS_HOME")
WEBOTS_HOME = "../../webots"

-- path to the Nao crosscompiler and libs
-- default: os.getenv("NAO_CTC")
NAO_CTC = nil
-- or set both explicitely
-- default: NAO_CTC .. "/compiler"
COMPILER_PATH_NAO = nil
-- default: NAO_CTC .. "/extern"
EXTERN_PATH_NAO = nil


-- naoqi toolchain needed to compile the NaoSMAL
-- default: os.getenv("AL_DIR")
AL_DIR = "../../naoqi-sdk-1.14.1-linux32"

-- example: add additional dirs for both platforms
--if PLATFORM == "Nao" then
--	PATH:includedirs {"my/nao/includes/path"}
--	PATH:libdirs {"my/nao/libs/path"}
--else
--	PATH:includedirs {"my/native/includes/path"}
--	PATH:libdirs {"my/native/libs/path"}
--end
