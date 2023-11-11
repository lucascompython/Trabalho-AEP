# Devido ao suporte limitado de Makefiles no Windows, este script foi criado para simular o comportamento de um Makefile no Windows (MSVC).
param (
    [switch]$clean = $false,
    [switch]$release = $false,
    [switch]$run = $false
)
$CFLAGS = "/Wall /WX /Iexternal\yyjson\src" # Mostrar todos os avisos e considerar avisos como erros
$LDFLAGS = ""
$sourceDir = "src"
$outputDir = "build"
$target = "gestao_stocks.exe"

if ($clean) {
    if (Test-Path "build") {
        Remove-Item -Recurse -Force -Path "build"
    }
    return
}


if ($release) {
    $CFLAGS += "-O2 -Z7 -MD"
    $LDFLAGS += ""
}

if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Path "build"
}


# Get the list of .c files in the source directory and its subdirectories
$c_files = Get-ChildItem -Path $sourceDir -Recurse -Include "*.c" 
foreach ($file in $c_files) {
    # Get the relative path of the file
    $relativePath = $file.FullName -replace [regex]::Escape($PWD.Path) + '\\' 
    # Replace the source directory with the output directory
    $outputPath = $relativePath -replace [regex]::Escape($sourceDir), $outputDir
    # Replace the .c extension with .obj
    $outputPath = $outputPath -replace '\.c$', '.obj'
    # Create the output directory if it doesn't exist
    $outputDir = Split-Path $outputPath
    # Compile the file
    
    "cl.exe $CFLAGS $relativePath -c -Fo:$outputPath" | Invoke-Expression
    # Se o ultimo comando falhou, sair.
    if ($LASTEXITCODE -ne 0) {
        exit
    }
}

# Link the object files
cl.exe $LDFLAGS $outputDir\*.obj -link -out:$outputDir\$target

if ($run) {
    & ".\$outputDir\$target"
}

