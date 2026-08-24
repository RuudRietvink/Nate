
#include "NateParser.h"
#include "NateCode.h"
#include "core/cpp/Rational.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <process.h>
#include <string>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

int parse(const std::string& aIn, const std::string& aOut, nate::NateParser::FileType aFileType = nate::NateParser::FileType::Normal)
{
	std::ifstream in(aIn);
	std::ofstream out(aOut);
    nate::NateCode coder;
	nate::NateParser nate(coder, aIn, in, out, aFileType);
	auto parseResult = nate.parseAndCode();
	std::cerr << "Errors: " << nate.errorCount() << ", Warnings: " << nate.warningCount() << std::endl;
	return (parseResult != 0 || nate.errorCount() != 0) ? 1 : 0;
}

	std::string errorMessage(int errorCode)
	{
#ifdef _WIN32
		char buffer[256] = {};
		if (strerror_s(buffer, sizeof(buffer), errorCode) != 0)
		{
			return "unknown error";
		}
		return buffer;
#else
		return std::strerror(errorCode);
#endif
	}

namespace
{
	namespace fs = std::filesystem;

	struct Options
	{
		std::vector<fs::path> sources;
		std::vector<std::string> compileArgs;
		std::vector<std::string> linkArgs;
		std::optional<fs::path> objectOutput;
		std::optional<fs::path> linkOutput;
		std::optional<fs::path> objectOutputDirectory;
		bool compileOnly = false;
	};

	struct NativeToolchain
	{
		fs::path compiler;
		fs::path linker;
		std::vector<fs::path> includeDirs;
		std::vector<fs::path> libDirs;
		std::vector<fs::path> pathDirs;
	};

	bool startsWith(const std::string& value, const std::string& prefix)
	{
		return value.rfind(prefix, 0) == 0;
	}

	std::string environmentValue(const char* name)
	{
#ifdef _WIN32
		char* value = nullptr;
		size_t length = 0;
		if (_dupenv_s(&value, &length, name) != 0 || value == nullptr)
		{
			return {};
		}

		std::string result(value);
		free(value);
		return result;
#else
		const char* value = std::getenv(name);
		return value ? value : "";
#endif
	}

	std::string quoteArgument(const std::string& value)
	{
		std::string escaped;
		escaped.reserve(value.size() + 2);
		for (char ch : value)
		{
			if (ch == '"')
			{
				escaped += '\\';
			}
			escaped += ch;
		}
		return '"' + escaped + '"';
	}

	std::string buildCommand(const std::vector<std::string>& args)
	{
		std::string command;
		for (const auto& arg : args)
		{
			if (!command.empty())
			{
				command += ' ';
			}
			command += quoteArgument(arg);
		}
		return command;
	}

	std::string pathList(const std::vector<fs::path>& paths)
	{
		std::string result;
		for (const auto& path : paths)
		{
			if (path.empty())
			{
				continue;
			}

			if (!result.empty())
			{
#ifdef _WIN32
				result += ';';
#else
				result += ':';
#endif
			}
			result += path.string();
		}
		return result;
	}

	int runCommand(const std::vector<std::string>& args)
	{
		if (args.empty())
		{
			return 1;
		}

		const auto command = buildCommand(args);
		std::cerr << command << std::endl;

#ifdef _WIN32
		STARTUPINFOA startupInfo = {};
		startupInfo.cb = sizeof(startupInfo);
		PROCESS_INFORMATION processInfo = {};
		std::vector<char> mutableCommand(command.begin(), command.end());
		mutableCommand.push_back('\0');

		if (!CreateProcessA(args.front().c_str(), mutableCommand.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startupInfo, &processInfo))
		{
			std::cerr << "failed to start native tool '" << args.front() << "': " << errorMessage(GetLastError()) << std::endl;
			return 1;
		}

		WaitForSingleObject(processInfo.hProcess, INFINITE);
		DWORD exitCode = 0;
		GetExitCodeProcess(processInfo.hProcess, &exitCode);
		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);

		if (exitCode != 0)
		{
			std::cerr << "native tool '" << args.front() << "' failed with exit code " << exitCode << std::endl;
			return 1;
		}
		return 0;
#else
		const int result = std::system(command.c_str());
		if (result != 0)
		{
			std::cerr << "native tool failed with exit code " << result << std::endl;
			return 1;
		}
		return 0;
#endif
	}

	fs::path generatedCppPath(const fs::path& outDir, const fs::path& sourcePath)
	{
		return outDir / sourcePath.filename().replace_extension(".cpp");
	}

	fs::path objectPathFor(const fs::path& outDir, const fs::path& sourcePath, const Options& options)
	{
		if (options.objectOutput && options.sources.size() == 1)
		{
			return *options.objectOutput;
		}

		const auto& objectDir = options.objectOutputDirectory ? *options.objectOutputDirectory : outDir;

#ifdef _WIN32
		const char* extension = ".obj";
#else
		const char* extension = ".o";
#endif
		return objectDir / sourcePath.filename().replace_extension(extension);
	}

	fs::path executablePathFor(const fs::path& outDir, const Options& options)
	{
		if (options.linkOutput)
		{
			return *options.linkOutput;
		}

#ifdef _WIN32
		const char* extension = ".exe";
#else
		const char* extension = "";
#endif
		return outDir / (options.sources.front().stem().string() + extension);
	}

	void appendIncludeDir(std::vector<fs::path>& includeDirs, const fs::path& includeDir)
	{
		if (includeDir.empty())
		{
			return;
		}

		for (const auto& existingDir : includeDirs)
		{
			if (existingDir == includeDir)
			{
				return;
			}
		}

		includeDirs.push_back(includeDir);
	}

	void ensureGeneratedSupportSources(const fs::path& createdDir)
	{
		const fs::path baseObjectHeader = createdDir / "BaseObject.h";
		const fs::path baseObjectCpp = createdDir / "BaseObject.cpp";
		if (fs::exists(baseObjectHeader) && !fs::exists(baseObjectCpp))
		{
			std::ifstream in(baseObjectHeader);
			const std::string headerContent((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
			const std::string className = headerContent.find("class BaseObject_") != std::string::npos ? "BaseObject_" : "BaseObject";
			if (className != "BaseObject")
			{
				return;
			}
			const std::string initializeName = headerContent.find("initialize__E_O_me__") != std::string::npos ? "initialize__E_O_me__" : "initialize_E_O_me__";

			std::ofstream out(baseObjectCpp);
			out << "#include \"BaseObject.h\"\n\n";
			out << "class " << className << "::__impl\n";
			out << "{\n";
			out << "private:\n";
			out << "\tfriend class " << className << ";\n";
			out << "\t" << className << "* me;\n";
			out << "public:\n";
			out << "\t__impl(" << className << "* aMe) : me(aMe) {}\n";
			out << "};\n\n";
			out << className << "::" << className << "()\n";
			out << "\t: _impl(new __impl(this)) {}\n";
			out << className << "::~" << className << "() { delete _impl; }\n";
			out << "void " << className << "::" << initializeName << "()\n";
			out << "{\n";
			out << "}\n";
		}
	}

	void appendUniquePath(std::vector<fs::path>& paths, const fs::path& path)
	{
		if (path.empty())
		{
			return;
		}

		for (const auto& existingPath : paths)
		{
			if (existingPath == path)
			{
				return;
			}
		}

		paths.push_back(path);
	}

#ifdef _WIN32
	void appendIfExists(std::vector<fs::path>& paths, const fs::path& path)
	{
		if (fs::exists(path))
		{
			appendUniquePath(paths, path);
		}
	}

	std::string targetArchitecture(const fs::path& toolDirectory)
	{
		const auto arch = toolDirectory.filename().string();
		return arch == "x86" ? "x86" : "x64";
	}

	std::optional<std::string> latestDirectoryName(const fs::path& root)
	{
		if (!fs::exists(root) || !fs::is_directory(root))
		{
			return std::nullopt;
		}

		std::vector<std::string> names;
		for (const auto& entry : fs::directory_iterator(root))
		{
			if (entry.is_directory())
			{
				names.push_back(entry.path().filename().string());
			}
		}

		if (names.empty())
		{
			return std::nullopt;
		}

		std::sort(names.begin(), names.end(), std::greater<>());
		return names.front();
	}

	std::vector<fs::path> msvcToolDirectories(const fs::path& msvcRoot)
	{
		std::vector<fs::path> toolDirectories;
		if (!fs::exists(msvcRoot) || !fs::is_directory(msvcRoot))
		{
			return toolDirectories;
		}

		std::vector<fs::path> versionDirectories;
		for (const auto& entry : fs::directory_iterator(msvcRoot))
		{
			if (entry.is_directory())
			{
				versionDirectories.push_back(entry.path());
			}
		}

		std::sort(versionDirectories.begin(), versionDirectories.end(), [](const fs::path& left, const fs::path& right)
		{
			return left.filename().string() > right.filename().string();
		});

		for (const auto& versionDirectory : versionDirectories)
		{
			for (const auto& hostTarget : {
				fs::path("bin") / "Hostx64" / "x64",
				fs::path("bin") / "Hostx86" / "x86",
				fs::path("bin") / "Hostx64" / "x86",
				fs::path("bin") / "Hostx86" / "x64"
				})
			{
				const fs::path toolDirectory = versionDirectory / hostTarget;
				if (fs::exists(toolDirectory))
				{
					toolDirectories.push_back(toolDirectory);
				}
			}
		}

		return toolDirectories;
	}

	std::optional<NativeToolchain> resolveWindowsToolchain()
	{
		std::vector<fs::path> candidateRoots;

		const auto cxx = environmentValue("CXX");
		if (!cxx.empty())
		{
			const fs::path compilerPath = cxx;
			const fs::path linkerPath = compilerPath.parent_path() / "link.exe";
			return NativeToolchain{ compilerPath, fs::exists(linkerPath) ? linkerPath : fs::path("link"), {}, {}, { compilerPath.parent_path() } };
		}

		const auto vcToolsInstallDir = environmentValue("VCToolsInstallDir");
		if (!vcToolsInstallDir.empty())
		{
			appendUniquePath(candidateRoots, fs::path(vcToolsInstallDir).parent_path());
		}

		const auto vsInstallDir = environmentValue("VSINSTALLDIR");
		if (!vsInstallDir.empty())
		{
			appendUniquePath(candidateRoots, fs::path(vsInstallDir) / "VC" / "Tools" / "MSVC");
		}

		for (const auto& programFilesEnv : { "ProgramFiles", "ProgramFiles(x86)" })
		{
			const auto programFiles = environmentValue(programFilesEnv);
			if (programFiles.empty())
			{
				continue;
			}

			const fs::path visualStudioRoot = fs::path(programFiles) / "Microsoft Visual Studio";
			for (const auto& version : { "18", "17", "Preview" })
			{
				for (const auto& edition : { "Community", "Professional", "Enterprise", "BuildTools" })
				{
					appendUniquePath(candidateRoots, visualStudioRoot / version / edition / "VC" / "Tools" / "MSVC");
				}
			}
		}

		for (const auto& candidateRoot : candidateRoots)
		{
			for (const auto& toolDirectory : msvcToolDirectories(candidateRoot))
			{
				const fs::path compilerPath = toolDirectory / "cl.exe";
				const fs::path linkerPath = toolDirectory / "link.exe";
				if (fs::exists(compilerPath) && fs::exists(linkerPath))
				{
					NativeToolchain toolchain{ compilerPath, linkerPath };
					toolchain.pathDirs.push_back(toolDirectory);

					const fs::path msvcVersionDir = toolDirectory.parent_path().parent_path().parent_path();
					appendIfExists(toolchain.includeDirs, msvcVersionDir / "include");
					appendIfExists(toolchain.libDirs, msvcVersionDir / "lib" / targetArchitecture(toolDirectory));

					const auto universalCrtSdkDir = environmentValue("UniversalCRTSdkDir");
					const auto ucrtVersion = environmentValue("UCRTVersion");
					if (!universalCrtSdkDir.empty() && !ucrtVersion.empty())
					{
						const fs::path sdkRoot = universalCrtSdkDir;
						appendIfExists(toolchain.includeDirs, sdkRoot / "Include" / ucrtVersion / "ucrt");
						appendIfExists(toolchain.libDirs, sdkRoot / "Lib" / ucrtVersion / "ucrt" / targetArchitecture(toolDirectory));
					}

					const auto windowsSdkDir = environmentValue("WindowsSdkDir");
					const auto windowsSdkVersion = environmentValue("WindowsSDKVersion");
					if (!windowsSdkDir.empty() && !windowsSdkVersion.empty())
					{
						const fs::path sdkRoot = windowsSdkDir;
						appendIfExists(toolchain.includeDirs, sdkRoot / "Include" / windowsSdkVersion / "shared");
						appendIfExists(toolchain.includeDirs, sdkRoot / "Include" / windowsSdkVersion / "um");
						appendIfExists(toolchain.includeDirs, sdkRoot / "Include" / windowsSdkVersion / "winrt");
						appendIfExists(toolchain.includeDirs, sdkRoot / "Include" / windowsSdkVersion / "cppwinrt");
						appendIfExists(toolchain.libDirs, sdkRoot / "Lib" / windowsSdkVersion / "um" / targetArchitecture(toolDirectory));
					}

					if (toolchain.includeDirs.size() <= 1 || toolchain.libDirs.empty())
					{
						std::vector<fs::path> windowsKitRoots;
						for (const auto& programFilesEnv : { "ProgramFiles(x86)", "ProgramFiles" })
						{
							const auto programFiles = environmentValue(programFilesEnv);
							if (!programFiles.empty())
							{
								appendUniquePath(windowsKitRoots, fs::path(programFiles) / "Windows Kits" / "10");
							}
						}

						for (const auto& windowsKitRoot : windowsKitRoots)
						{
							const auto includeVersion = latestDirectoryName(windowsKitRoot / "Include");
							if (includeVersion)
							{
								appendIfExists(toolchain.includeDirs, windowsKitRoot / "Include" / *includeVersion / "ucrt");
								appendIfExists(toolchain.includeDirs, windowsKitRoot / "Include" / *includeVersion / "shared");
								appendIfExists(toolchain.includeDirs, windowsKitRoot / "Include" / *includeVersion / "um");
								appendIfExists(toolchain.includeDirs, windowsKitRoot / "Include" / *includeVersion / "winrt");
								appendIfExists(toolchain.includeDirs, windowsKitRoot / "Include" / *includeVersion / "cppwinrt");
							}

							const auto libVersion = latestDirectoryName(windowsKitRoot / "Lib");
							if (libVersion)
							{
								appendIfExists(toolchain.libDirs, windowsKitRoot / "Lib" / *libVersion / "ucrt" / targetArchitecture(toolDirectory));
								appendIfExists(toolchain.libDirs, windowsKitRoot / "Lib" / *libVersion / "um" / targetArchitecture(toolDirectory));
							}
						}
					}

					return toolchain;
				}
			}
		}

		return std::nullopt;
	}
#endif

	std::vector<fs::path> includeDirsForSource(const std::vector<fs::path>& baseIncludeDirs, const fs::path& sourcePath)
	{
		std::vector<fs::path> includeDirs = baseIncludeDirs;
		const fs::path sourceDir = sourcePath.has_parent_path() ? sourcePath.parent_path() : fs::current_path();
		appendIncludeDir(includeDirs, sourceDir);
		appendIncludeDir(includeDirs, sourceDir / "created");
		return includeDirs;
	}

	void appendCppFilesFromDirectory(std::vector<fs::path>& sourceFiles, const fs::path& directory)
	{
		if (!fs::exists(directory) || !fs::is_directory(directory))
		{
			return;
		}

		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (entry.is_regular_file() && entry.path().extension() == ".cpp")
			{
				appendUniquePath(sourceFiles, entry.path());
			}
		}
	}

	fs::path auxiliaryObjectPathFor(const fs::path& repoRoot, const fs::path& outDir, const fs::path& sourcePath)
	{
		fs::path relativePath;
		try
		{
			relativePath = fs::relative(sourcePath, repoRoot);
		}
		catch (const fs::filesystem_error&)
		{
			relativePath = sourcePath.filename();
		}

		std::string objectName;
		for (const auto& part : relativePath)
		{
			if (!objectName.empty())
			{
				objectName += '-';
			}
			objectName += part.string();
		}

		for (auto& ch : objectName)
		{
			if (ch == '\\' || ch == '/' || ch == ':' || ch == ' ')
			{
				ch = '-';
			}
		}

#ifdef _WIN32
		const char* extension = ".obj";
#else
		const char* extension = ".o";
#endif
		return outDir / fs::path(objectName).replace_extension(extension);
	}

	bool isLinkerOnlyPosixArg(const std::string& arg)
	{
		return startsWith(arg, "-l") || startsWith(arg, "-L") || startsWith(arg, "-Wl,")
			|| arg == "-shared" || arg == "-static" || arg == "-rdynamic" || arg == "-s";
	}

	void printUsage()
	{
		std::cerr << "usage: Nate [-c] [-o output] source.ns [source.ns ...] [native compiler options]" << std::endl;
#ifdef _WIN32
		std::cerr << "       use /link followed by native linker options" << std::endl;
#else
		std::cerr << "       linker options such as -l, -L, -Wl,... are forwarded during linking" << std::endl;
#endif
	}

	std::optional<Options> parseArguments(int argc, char* argv[])
	{
		Options options;
		bool linkSection = false;

		for (int i = 1; i < argc; ++i)
		{
			std::string arg = argv[i];

			if (arg == "-c")
			{
				options.compileOnly = true;
				continue;
			}

			if (arg == "-o")
			{
				if (i + 1 >= argc)
				{
					std::cerr << "missing value for -o" << std::endl;
					return std::nullopt;
				}

				const fs::path output = argv[++i];
				if (options.compileOnly)
				{
					options.objectOutput = output;
				}
				else
				{
					options.linkOutput = output;
				}
				continue;
			}

#ifdef _WIN32
			if (arg == "/link")
			{
				linkSection = true;
				continue;
			}

			if (startsWith(arg, "/Fo"))
			{
				std::string value = arg.substr(3);
				if (value.empty())
				{
					if (i + 1 >= argc)
					{
						std::cerr << "missing value for /Fo" << std::endl;
						return std::nullopt;
					}
					value = argv[++i];
				}

				fs::path output = value;
				if (output.has_extension())
				{
					options.objectOutput = output;
				}
				else
				{
					options.objectOutputDirectory = output;
				}
				continue;
			}

			if (startsWith(arg, "/Fe"))
			{
				std::string value = arg.substr(3);
				if (value.empty())
				{
					if (i + 1 >= argc)
					{
						std::cerr << "missing value for /Fe" << std::endl;
						return std::nullopt;
					}
					value = argv[++i];
				}

				options.linkOutput = value;
				continue;
			}
#endif

			if (fs::path(arg).extension() == ".ns")
			{
				options.sources.emplace_back(arg);
				continue;
			}

#ifndef _WIN32
			if (arg == "-Xlinker")
			{
				if (i + 1 >= argc)
				{
					std::cerr << "missing value for -Xlinker" << std::endl;
					return std::nullopt;
				}

				options.linkArgs.push_back(arg);
				options.linkArgs.push_back(argv[++i]);
				continue;
			}

			if (isLinkerOnlyPosixArg(arg))
			{
				options.linkArgs.push_back(arg);
				continue;
			}
#endif

			if (linkSection)
			{
				options.linkArgs.push_back(arg);
			}
			else
			{
				options.compileArgs.push_back(arg);
			}
		}

		if (options.sources.empty())
		{
			printUsage();
			return std::nullopt;
		}

		if (options.objectOutput && options.sources.size() > 1)
		{
			std::cerr << "a single object output may only be used with one .ns source" << std::endl;
			return std::nullopt;
		}

		return options;
	}

	int compileCpp(const fs::path& compiler, const std::vector<fs::path>& includeDirs, const std::vector<std::string>& compileArgs,
		const std::vector<fs::path>& toolchainIncludeDirs, const fs::path& cppPath, const fs::path& objectPath)
	{
		fs::create_directories(objectPath.parent_path());

		std::vector<std::string> command;
		command.push_back(compiler.string());

#ifdef _WIN32
		command.push_back("/nologo");
		command.push_back("/EHsc");
		command.push_back("/std:c++latest");
		command.push_back("/c");
		for (const auto& includeDir : includeDirs)
		{
			command.push_back("/I" + includeDir.string());
		}
		for (const auto& includeDir : toolchainIncludeDirs)
		{
			command.push_back("/I" + includeDir.string());
		}
		for (const auto& arg : compileArgs)
		{
			command.push_back(arg);
		}
		command.push_back("/Fo" + objectPath.string());
		command.push_back(cppPath.string());
#else
		command.push_back("-std=c++23");
		command.push_back("-c");
		for (const auto& includeDir : includeDirs)
		{
			command.push_back("-I" + includeDir.string());
		}
		for (const auto& includeDir : toolchainIncludeDirs)
		{
			command.push_back("-I" + includeDir.string());
		}
		for (const auto& arg : compileArgs)
		{
			command.push_back(arg);
		}
		command.push_back(cppPath.string());
		command.push_back("-o");
		command.push_back(objectPath.string());
#endif

		if (runCommand(command) != 0)
		{
			return 1;
		}

		if (!fs::exists(objectPath))
		{
			std::cerr << "compiler reported success but no object file was created: " << objectPath.string() << std::endl;
			return 1;
		}

		return 0;
	}

	int linkObjects(const fs::path& linker, const fs::path& outPath, const std::vector<fs::path>& objects,
		const std::vector<std::string>& compileArgs, const std::vector<std::string>& linkArgs, const std::vector<fs::path>& toolchainLibDirs)
	{
		fs::create_directories(outPath.parent_path());

		std::vector<std::string> command;

#ifdef _WIN32
		command.push_back(linker.string());
		command.push_back("/NOLOGO");
		for (const auto& object : objects)
		{
			command.push_back(object.string());
		}
		for (const auto& libDir : toolchainLibDirs)
		{
			command.push_back("/LIBPATH:" + libDir.string());
		}
		command.push_back("/OUT:" + outPath.string());
		for (const auto& arg : linkArgs)
		{
			command.push_back(arg);
		}
#else
		command.push_back(linker.string());
		for (const auto& arg : compileArgs)
		{
			command.push_back(arg);
		}
		for (const auto& object : objects)
		{
			command.push_back(object.string());
		}
		for (const auto& arg : linkArgs)
		{
			command.push_back(arg);
		}
		for (const auto& libDir : toolchainLibDirs)
		{
			command.push_back("-L" + libDir.string());
		}
		command.push_back("-o");
		command.push_back(outPath.string());
#endif

		return runCommand(command);
	}
}

int main(int argc, char* argv[])
{
	namespace fs = std::filesystem;

	Core::enableUtf8Console();
	setvbuf(stdout, nullptr, _IOFBF, 1000);
	setvbuf(stderr, nullptr, _IOFBF, 1000);

	const auto options = parseArguments(argc, argv);
	if (!options)
	{
		return 1;
	}

	const fs::path repoRoot = fs::current_path();
	const fs::path libraryRoot = repoRoot / "NateLib";
	const fs::path coreDir = libraryRoot / "core";
	const fs::path coreCppDir = coreDir / "cpp";
	const fs::path createdDir = coreDir / "created";
	const fs::path outDir = repoRoot / "Out";
	const std::vector<fs::path> includeDirs = { coreCppDir, createdDir, libraryRoot / "input" / "created", repoRoot / "utf8" };

#ifdef _WIN32
	const auto toolchain = resolveWindowsToolchain();
	if (!toolchain)
	{
		std::cerr << "unable to locate MSVC tools; set CXX or launch from a Visual Studio developer environment" << std::endl;
		return 1;
	}
	const fs::path compiler = toolchain->compiler;
	const fs::path linker = toolchain->linker;
	const std::vector<fs::path> toolchainIncludeDirs = toolchain->includeDirs;
	const std::vector<fs::path> toolchainLibDirs = toolchain->libDirs;
	if (!toolchain->includeDirs.empty() && _putenv_s("INCLUDE", pathList(toolchain->includeDirs).c_str()) != 0)
	{
		std::cerr << "unable to configure INCLUDE for MSVC" << std::endl;
		return 1;
	}
	if (!toolchain->libDirs.empty() && _putenv_s("LIB", pathList(toolchain->libDirs).c_str()) != 0)
	{
		std::cerr << "unable to configure LIB for MSVC" << std::endl;
		return 1;
	}
	std::vector<fs::path> pathEntries = toolchain->pathDirs;
	const auto currentPath = environmentValue("PATH");
	if (!currentPath.empty())
	{
		pathEntries.push_back(currentPath);
	}
	if (!pathEntries.empty() && _putenv_s("PATH", pathList(pathEntries).c_str()) != 0)
	{
		std::cerr << "unable to configure PATH for MSVC" << std::endl;
		return 1;
	}
#else
	const auto cxx = environmentValue("CXX");
	const fs::path compiler = cxx.empty() ? "c++" : fs::path(cxx);
	const fs::path linker = compiler;
	const std::vector<fs::path> toolchainIncludeDirs;
	const std::vector<fs::path> toolchainLibDirs;
#endif

	fs::create_directories(createdDir);
	fs::create_directories(outDir);
	ensureGeneratedSupportSources(libraryRoot / "input" / "created");

	const auto originalPath = fs::current_path();
	fs::current_path(coreDir);
	if (parse("import/BaseObject.ns", (createdDir / "BaseObject.cpp").string(), nate::NateParser::FileType::ObjectImpl) != 0)
	{
		return 1;
	}
	std::cerr << "generated " << (createdDir / "BaseObject.cpp").string() << std::endl;
	if (parse("import/File-Input.ns", (createdDir / "File-Input.cpp").string(), nate::NateParser::FileType::ObjectImpl) != 0)
	{
		return 1;
	}
	std::cerr << "generated " << (createdDir / "File-Input.cpp").string() << std::endl;
	fs::current_path(originalPath);

	std::vector<fs::path> objects;
	for (const auto& source : options->sources)
	{
		const fs::path cppPath = generatedCppPath(outDir, source);
		const auto sourceIncludeDirs = includeDirsForSource(includeDirs, source);
		const fs::path sourceDir = source.has_parent_path() ? source.parent_path() : repoRoot;
		ensureGeneratedSupportSources(sourceDir / "created");
		if (parse(source.string(), cppPath.string()) != 0)
		{
			return 1;
		}
		std::cerr << "generated " << cppPath.string() << std::endl;

		const fs::path objectPath = objectPathFor(outDir, source, *options);
		if (compileCpp(compiler, sourceIncludeDirs, options->compileArgs, toolchainIncludeDirs, cppPath, objectPath) != 0)
		{
			return 1;
		}
		std::cerr << "compiled " << objectPath.string() << std::endl;

		objects.push_back(objectPath);
	}

	if (options->compileOnly)
	{
		std::cerr << "compile finished" << std::endl;
		return 0;
	}

	std::vector<fs::path> auxiliarySources;
	appendCppFilesFromDirectory(auxiliarySources, createdDir);
	appendCppFilesFromDirectory(auxiliarySources, libraryRoot / "input" / "created");
	for (const auto& source : options->sources)
	{
		const fs::path sourceDir = source.has_parent_path() ? source.parent_path() : repoRoot;
		if (sourceDir != repoRoot)
		{
			ensureGeneratedSupportSources(sourceDir / "created");
		}
		appendCppFilesFromDirectory(auxiliarySources, sourceDir / "created");
	}
	appendUniquePath(auxiliarySources, coreCppDir / "Core.cpp");
	appendUniquePath(auxiliarySources, coreCppDir / "Rational.cpp");

	for (const auto& auxiliarySource : auxiliarySources)
	{
		const auto auxiliaryIncludeDirs = includeDirsForSource(includeDirs, auxiliarySource);
		const fs::path auxiliaryObjectPath = auxiliaryObjectPathFor(repoRoot, outDir, auxiliarySource);
		if (compileCpp(compiler, auxiliaryIncludeDirs, options->compileArgs, toolchainIncludeDirs, auxiliarySource, auxiliaryObjectPath) != 0)
		{
			return 1;
		}
		std::cerr << "compiled " << auxiliaryObjectPath.string() << std::endl;
		objects.push_back(auxiliaryObjectPath);
	}

	const fs::path executablePath = executablePathFor(outDir, *options);
	if (linkObjects(linker, executablePath, objects, options->compileArgs, options->linkArgs, toolchainLibDirs) != 0)
	{
		return 1;
	}
	std::cerr << "linked " << executablePath.string() << std::endl;
	return 0;
}