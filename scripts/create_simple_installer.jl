#!/usr/bin/env julia

function create_installer(
    package_name::String,
    version::String,
    publisher::String,
    description::String;
    program::String,              # Main executable 
    dlls::Vector{String}=String[],# DLLs to include
    assets::String="assets",      # Assets folder
	 icon::String="",              # Icon file (.ico format)
    output_dir::String=".",       # Output directory
    inno_setup_path::String="C:/Program Files (x86)/Inno Setup 6" # Inno Setup location
)
    # Ensure output directory exists
    mkpath(output_dir)
    
    # Path to Inno Setup compiler
    iscc_exe = joinpath(inno_setup_path, "ISCC.exe")
    
    # Create Inno Setup script
    iss_file = joinpath(output_dir, "$(package_name)_installer.iss")
    
    # Convert executable and DLL paths to absolute paths
    abs_program = isabspath(program) ? program : abspath(program)
    abs_dlls = [isabspath(dll) ? dll : abspath(dll) for dll in dlls]
    abs_assets = isabspath(assets) ? assets : abspath(assets)
    
    # Get directory of main executable
    program_dir = dirname(abs_program)

	 # Process icon file
    icon_param = ""
    if !isempty(icon) && isfile(icon)
        abs_icon = isabspath(icon) ? icon : abspath(icon)
        icon_param = "SetupIconFile=$(abs_icon)"
    end
    
    # Create Inno Setup script
    open(iss_file, "w") do io
        write(io, """
        [Setup]
        AppName=$(package_name)
        AppVersion=$(version)
        AppPublisher=$(publisher)
        AppPublisherURL=https://github.com/daniel-schwarzenbach/Senpai-App-Engine
        AppSupportURL=https://github.com/daniel-schwarzenbach/Senpai-App-Engine
        AppUpdatesURL=https://github.com/daniel-schwarzenbach/Senpai-App-Engine
        DefaultDirName={commonpf}\\$(package_name)
        DefaultGroupName=$(package_name)
        OutputDir=$(output_dir)
        OutputBaseFilename=$(package_name)_$(version)_setup
        Compression=lzma
        SolidCompression=yes
        $(icon_param)
        
        [Languages]
        Name: "english"; MessagesFile: "compiler:Default.isl"
        
        [Tasks]
        Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"
        Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"

		  
        
        [Files]
        ; Main executable
        Source: "$(abs_program)"; DestDir: "{app}"; Flags: ignoreversion
		  
        """)
        
        # Add icon file to files section if provided
        if !isempty(icon) && isfile(icon)
            write(io, "Source: \"$(abs_icon)\"; DestDir: \"{app}\"; Flags: ignoreversion\n")
        end
        
        # Add DLLs
        for dll in abs_dlls
            write(io, "Source: \"$(dll)\"; DestDir: \"{app}\"; Flags: ignoreversion\n")
        end
        
        # Add assets folder (recursive)
        write(io, "Source: \"$(abs_assets)\\*\"; DestDir: \"{app}\\assets\"; Flags: ignoreversion recursesubdirs createallsubdirs\n")

		  # Add write permissions for the assets/data folder
		  write(io, """
		  [Dirs]
		  Name: "{app}\\assets\\data"; Permissions: users-modify
		  """)
        
        # Add icons and shortcuts
        if !isempty(icon) && isfile(icon)
            write(io, """
            [Icons]
            Name: "{group}\\$(package_name)"; Filename: "{app}\\$(basename(abs_program))"; IconFilename: "{app}\\$(basename(abs_icon))"
            Name: "{commondesktop}\\$(package_name)"; Filename: "{app}\\$(basename(abs_program))"; IconFilename: "{app}\\$(basename(abs_icon))"; Tasks: desktopicon
            """)
        else
            write(io, """
            [Icons]
            Name: "{group}\\$(package_name)"; Filename: "{app}\\$(basename(abs_program))"
            Name: "{commondesktop}\\$(package_name)"; Filename: "{app}\\$(basename(abs_program))"; Tasks: desktopicon
            """)
        end
        
        # ...rest of the code...

		  
        
        
    end
    
    # Run Inno Setup compiler
	 println("Creating installer...")
	 run(`"$iscc_exe" "$iss_file"`)
	 
	 println("Installer created: $(output_dir)/$(package_name)_$(version)_setup.exe")
end

# Example usage with icon
create_installer(
    "Five Knights",
    "1.0",
    "FiveKnights Team",
    "Five Knights against King Fredric Game",
    program = "build/Game/Release/FiveKnights.exe",
    dlls = [
        "build/Game/Release/SDL3.dll", 
        "build/Game/Release/SDL3_image.dll",
        "build/Game/Release/SDL3_ttf.dll",
        "build/Game/Release/SDL3_mixer.dll"
    ],
    assets = "Game/assets",
    icon = "Game/assets/pics/KingBig.ico",  # Use your .ico file here
	 output_dir = "private",  # Output directory for the installer
    inno_setup_path = "C:/Program Files (x86)/Inno Setup 6"
)