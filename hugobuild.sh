#/bin/bash

hugo new site site
cd site
cd themes
git clone https://github.com/mrmierzejewski/hugo-theme-console.git
cd ..
echo "title = \"PFG report\"" > config.toml
echo "theme = \"hugo-theme-console\"" >> config.toml
cd ..

python3 hugo_generate.py pngmap.json build/ > site/content/index.md
cp build/*.png site/content/

cd site
hugo -D