# escape=`

# docker build -t "website" -f "c:\git\the-rights-machine\process\website\dockerfile" ".."
# docker run -it website

FROM microsoft/windowsservercore as buildtools
SHELL ["powershell.exe", "-ExecutionPolicy", "Bypass", "-Command"]
RUN Set-ExecutionPolicy Bypass -Scope Process -Force; `
    iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
RUN choco install microsoft-build-tools -y
RUN choco install dotnet4.6.1 -y
RUN choco install netfx-4.6.1-devpack -y
RUN choco install nuget.commandline -y
RUN nuget install Microsoft.Data.Tools.Msbuild -Version 10.0.61804.210
ENV MSBUILD_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin" `
    DATATOOLS_PATH="C:\Microsoft.Data.Tools.Msbuild.10.0.61804.210\lib\net46"
RUN $env:PATH = $env:DATATOOLS_PATH + ';' + $env:MSBUILD_PATH + ';' + $env:PATH; `
    [Environment]::SetEnvironmentVariable('PATH', $env:PATH, [EnvironmentVariableTarget]::Machine)
RUN setx SQLDBExtensionsRefPath $env:DATATOOLS_PATH /M
RUN setx SSDTPath $env:DATATOOLS_PATH /M

FROM buildtools AS builder
WORKDIR /src
#COPY ["database/database.sqlproj", "database/"]
#COPY ["database/*.sql", "database/"]
Add database database
RUN msbuild "database/database.sqlproj" /p:Configuration=Release /t:restore
COPY . .
WORKDIR "/src/database"
RUN msbuild "database.sqlproj" /p:Configuration=Release

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

# todo test database table copy
# setup c++ service
# copy and build C++ service, and install
# expose c++ service ports
# test website with database localhost
# unify login tables from net core to process app