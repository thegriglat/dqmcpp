#/bin/bash


mkdir -p public

cp -f build/*.png public
cp custom.css  public/
python3 hugo_generate.py pngmap.json build/ | pandoc -f markdown -t html -s -c custom.css > public/index.html
cd public; ls *.png | xargs -n 1 -P 4 -I {} convert {} -trim {}; cd -
