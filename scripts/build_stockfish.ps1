param(
    [string]$Variant = "generic"
)

$RootDir = Split-Path `
    -Parent `
    (Split-Path `
        -Parent `
        $MyInvocation.MyCommand.Path)


Write-Host "Building Stockfish variant: $Variant"


# Select gyp file
Copy-Item `
    "$RootDir\binding.$Variant.gyp" `
    "$RootDir\binding.gyp" `
    -Force


Set-Location $RootDir


# Clean previous build
if (Test-Path "build") {
    Remove-Item `
        -Recurse `
        -Force `
        "build"
}


# Configure
node-gyp configure


# Build
node-gyp build


# Store result
$OutputDir =
    "$RootDir\native\win32-$Variant"


New-Item `
    -ItemType Directory `
    -Force `
    $OutputDir | Out-Null


Copy-Item `
    "$RootDir\build\Release\stockfish.node" `
    "$OutputDir\stockfish.node" `
    -Force


Write-Host ""
Write-Host "Done:"
Write-Host "$OutputDir\stockfish.node"