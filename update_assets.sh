# we need to re-zip up the assets
# first remove old zip
rm -rf projectrpg-img.zip;
# then zip up the new assets
zip -r projectrpg-img.zip img audio fonts title_textures.txt game_textures.txt gameover_textures.txt;
# then copy the new zip to the website folder
cp projectrpg-img.zip ../mikedesu.github.io/.;
