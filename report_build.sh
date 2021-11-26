#/bin/bash

builddir="$1"
outdir="$2"
mkdir -p $outdir

cp -f $builddir/*.png $outdir
cp custom.css  $outdir/
python3 generate_md.py pngmap.json $builddir/ | pandoc -f markdown -t html -s -c custom.css --metadata title="DQMReport" > $outdir/index.html
cd $outdir; ls *.png | xargs -n 1 -P 4 -I {} convert {} -trim {}; cd -
