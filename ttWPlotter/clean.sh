echo "-> Cleaning libaries"
rm -f Selector/*.d
rm -f Selector/*.so
rm -f Selector/*.pcm

rm -f Plotter/*.d
rm -f Plotter/*.so
rm -f Plotter/*.pcm

echo "-> Removing old ROOT files"
rm -f HISTOS/*.root
