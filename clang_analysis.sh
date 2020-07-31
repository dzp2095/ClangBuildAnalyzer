cd ClangBuildAnalyzer
./cmake-build-release/ClangBuildAnalyzer --start ../AndroidDemo/effectAAR/.cxx
cd ../
#do the build
cd ClangBuildAnalyzer
./cmake-build-release/ClangBuildAnalyzer --stop ../AndroidDemo/effectAAR/.cxx result.data
./cmake-build-release/ClangBuildAnalyzer --analyze result.data
pip3 install openpyxl
python3 ./HandleJson.py
cd ../