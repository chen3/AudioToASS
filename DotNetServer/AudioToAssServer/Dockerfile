FROM mcr.microsoft.com/dotnet/core/aspnet:2.2-stretch-slim AS base
WORKDIR /app
EXPOSE 80

FROM mcr.microsoft.com/dotnet/core/sdk:2.2-stretch AS build
WORKDIR /src
COPY ["AudioToAssServer/AudioToAssServer.csproj", "AudioToAssServer/"]
RUN dotnet restore "AudioToAssServer/AudioToAssServer.csproj"
COPY . .
WORKDIR "/src/AudioToAssServer"
RUN dotnet build "AudioToAssServer.csproj" -c Release -o /app

FROM build AS publish
RUN dotnet publish "AudioToAssServer.csproj" -c Release -o /app

FROM base AS final
WORKDIR /app
COPY --from=publish /app .
ENTRYPOINT ["dotnet", "AudioToAssServer.dll"]