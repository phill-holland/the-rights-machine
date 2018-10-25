# escape=`

# docker build -t "website" -f "c:\git\the-rights-machine\process\website\dockerfile" ".."
# docker run -it website

FROM microsoft/dotnet-framework:4.7.1 as buildtools
SHELL ["powershell.exe", "-ExecutionPolicy", "Bypass", "-Command"]
RUN Set-ExecutionPolicy Bypass -Scope Process -Force; `
    iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
#
RUN choco install windows-sdk-10.1 -y
RUN choco install visualstudio2017buildtools -y --execution-timeout 5400 --package-parameters '--passive --wait --norestart --add Microsoft.VisualStudio.Workload.DataBuildTools --add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.Component.VC.Runtime.UCRTSDK --locale en-US'
RUN choco install dotnet4.6.1 -y
RUN choco install netfx-4.6.1-devpack -y
run choco install nasm -y
RUN choco install nuget.commandline -y
RUN nuget install Microsoft.Data.Tools.Msbuild -Version 10.0.61804.210
ENV MSBUILD_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin" `
    DATATOOLS_PATH="C:\Microsoft.Data.Tools.Msbuild.10.0.61804.210\lib\net46" `
	NASM_PATH="C:\Program Files\NASM" `
	NMAKE_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.15.26726\bin\Hostx64\x64" `
	WIN10_PATH="C:\Program Files (x86)\Windows Kits\10\bin\10.0.17134.0\x64" `
	INCLUDE="C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.15.26726\include;C:\Program Files (x86)\Windows Kits\10\Include\10.0.10240.0\ucrt;C:\Program Files (x86)\Windows Kits\10\Include\10.0.17134.0\um;C:\Program Files (x86)\Windows Kits\10\Include\10.0.17134.0\shared;"  `
	LIB_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Tools\MSVC\14.15.26726\lib\x64;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.10240.0\ucrt\x64;C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17134.0\um\x64;"
RUN $env:PATH = $env:DATATOOLS_PATH + ';' + $env:MSBUILD_PATH + ';' + $env:NASM_PATH + ';' + $env:NMAKE_PATH + ';' + $env:WIN10_PATH + ';' + $env:PATH; `
    [Environment]::SetEnvironmentVariable('PATH', $env:PATH, [EnvironmentVariableTarget]::Machine)
RUN [Environment]::SetEnvironmentVariable('LIB', $env:LIB_PATH, [EnvironmentVariableTarget]::Machine)
RUN [Environment]::SetEnvironmentVariable('LIBPATH', $env:LIB_PATH, [EnvironmentVariableTarget]::Machine)
RUN setx SQLDBExtensionsRefPath $env:DATATOOLS_PATH /M
RUN setx SSDTPath $env:DATATOOLS_PATH /M

##
### ********* CUDA TOOLS and SSL
##
WORKDIR /installer
#ADD https://developer.nvidia.com/compute/cuda/10.0/Prod/network_installers/cuda_10.0.130_win10_network toolkit.exe
ADD http://127.0.0.1/support/cuda_10.0.130_411.31_win10.exe toolkit.exe
RUN Start-Process 'c:\installer\toolkit.exe' '-s nvcc_10.0' -PassThru | Wait-Process;
RUN choco install 7zip.install -y
ENV ZIP_PATH="C:\Program Files\7-Zip"
RUN $env:PATH = $env:ZIP_PATH + ';' + $env:PATH; `
    [Environment]::SetEnvironmentVariable('PATH', $env:PATH, [EnvironmentVariableTarget]::Machine)
RUN 7z e toolkit.exe 'CUDAVisualStudioIntegration\extras\visual_studio_integration\MSBuildExtensions\*' -o'c:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\IDE\VC\VCTargets\BuildCustomizations'
#ADD https://www.openssl.org/source/openssl-1.1.1.tar.gz openssl.tar.gz
ADD http://127.0.0.1/support/openssl-1.1.1.tar.gz openssl.tar.gz
RUN 7z x openssl.tar.gz -oc:\installer
RUN 7z x openssl.tar -oc:\
#ADD http://strawberryperl.com/download/5.28.0.1/strawberry-perl-5.28.0.1-64bit.msi perl.msi
ADD http://127.0.0.1/support/strawberry-perl-5.28.0.1-64bit.msi perl.msi
RUN msiexec /i c:\\installer\\perl.msi /quiet /qn /norestart
WORKDIR /openssl-1.1.1
RUN perl Configure VC-WIN64A
RUN nmake
RUN nmake test
RUN nmake install

## ********
#
FROM buildtools AS builder
WORKDIR /src
Add database database
add process process
RUN msbuild "database/database.sqlproj" /p:Configuration=Release /t:restore
RUN msbuild "process/process.vcxproj" /p:Configuration=Release /t:restore
COPY . .
WORKDIR "/src/database"
RUN msbuild "database.sqlproj" /p:Configuration=Release
WORKDIR "/src/process"
RUN msbuild "process.vcxproj" /p:Configuration=Release /p:Platform=x64

#CMD ["powershell.exe", "-NoLogo", "-ExecutionPolicy", "Bypass"]

FROM microsoft/mssql-server-windows-express as sqlserver
WORKDIR c:\
ENV DATATOOLS_PATH="C:\Microsoft.Data.Tools.Msbuild.10.0.61804.210\lib\net46"
RUN Set-ExecutionPolicy Bypass -Scope Process -Force; `
    iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
RUN choco install nuget.commandline -y
RUN choco install aspnetcore-runtimepackagestore -y
RUN choco install dotnetcore-runtime -y
RUN nuget install Microsoft.Data.Tools.Msbuild -Version 10.0.61804.210; `
	$env:PATH = $env:DATATOOLS_PATH + ';' + $env:PATH; `
    [Environment]::SetEnvironmentVariable('PATH', $env:PATH, [EnvironmentVariableTarget]::Machine)
WORKDIR c:\init
EXPOSE 1433:1433
ENV ACCEPT_EULA "Y" `
	DATA_PATH "C:\\init" `
	SA_PASSWORD "Funjuice97?"
COPY --from=builder C:\src\Database\bin\Release\Database.dacpac .
RUN SqlPackage.exe /sf:Database.dacpac /a:Script /op:create.sql /p:CommentOutSetVarDeclarations=true /tsn:.\SQLEXPRESS /tdn:Process
WORKDIR c:\database
RUN Invoke-Sqlcmd -InputFile c:\init\create.sql -Variable "DatabaseName='Process'", "DefaultFilePrefix='Process'", "DefaultDataPath='c:\database\'", "DefaultLogPath='c:\database\'"
#CMD ["powershell.exe", "-NoLogo", "-ExecutionPolicy", "Bypass"]
#
FROM sqlserver AS base
WORKDIR /app
EXPOSE 54526
EXPOSE 44370

FROM microsoft/dotnet:2.1-sdk-nanoserver-1803 AS build
WORKDIR /src
COPY ["website/website.csproj", "website/"]
RUN dotnet restore "website/website.csproj"
COPY . .
WORKDIR "/src/website"
RUN dotnet build "website.csproj" -c Release -o /app

FROM build AS publish
RUN dotnet publish "website.csproj" -c Release -o /app

FROM base AS final
WORKDIR /app
COPY --from=publish /app .
ENTRYPOINT ["dotnet", "website.dll"]
##
# todo test database table copy
# setup c++ service
# copy and build C++ service, and install
# expose c++ service ports
# test website with database localhost
# unify login tables from net core to process app