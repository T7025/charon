set term epslatex

set key outside above left
set datafile separator comma
set grid xtics ytics mxtics mytics
set logscale xy 2
set xlabel "# bodies"
set ylabel "Speedup" offset 1.5,0
set xtics rotate by -30
set output "BHMTSpeedup.tex"
plot [48:1572864][0.5:*] "BarnesHutMultiThreadSpeedup1" title "1 core" with linespoints pt 7 lc rgb "#396AB1" lw 1.5, \
	"BarnesHutMultiThreadSpeedup2" title "2 cores" with linespoints pt 7 lc rgb "#DA7C30" lw 1.5, \
	"BarnesHutMultiThreadSpeedup3" title "3 cores" with linespoints pt 7 lc rgb "#3E9651" lw 1.5, \
	"BarnesHutMultiThreadSpeedup4" title "4 cores" with linespoints pt 7 lc rgb "#CC2529" lw 1.5, \
	"BarnesHutMultiThreadSpeedup5" title "5 cores" with linespoints pt 7 lc rgb "#535154" lw 1.5, \
	"BarnesHutMultiThreadSpeedup6" title "6 cores" with linespoints pt 7 lc rgb "#6B4C9A" lw 1.5
unset output

set output "BHOSpeedup.tex"
plot [48:1572864][*:1.5] "BarnesHutMultiThreadSpeedup1" title "Single thread CPU" with linespoints pt 7 lc rgb "#396AB1" lw 1.5, \
	"BarnesHutOffloadUniverseSpeedup" title "GPU Offload" with linespoints pt 7 lc rgb "#DA7C30" lw 1.5
unset output

set output "BHTSpeedup.tex"
plot [48:1572864][0.5:*] "BarnesHutMultiThreadSpeedup1" title "SFC based Barnes-Hut" with linespoints pt 7 lc rgb "#396AB1" lw 1.5, \
	"BarnesHutTreeUniverseSpeedup" title "Recursive Barnes-Hut" with linespoints pt 7 lc rgb "#DA7C30" lw 1.5
unset output
set xtics norotate
set output "BFMTSpeedup.tex"
plot [48:49152][0.25:*] "BruteForceMultiThreadSpeedup1" title "1 core" with linespoints pt 7 lc rgb "#396AB1" lw 1.5, \
	"BruteForceMultiThreadSpeedup2" title "2 cores" with linespoints pt 7 lc rgb "#DA7C30" lw 1.5, \
	"BruteForceMultiThreadSpeedup3" title "3 cores" with linespoints pt 7 lc rgb "#3E9651" lw 1.5, \
	"BruteForceMultiThreadSpeedup4" title "4 cores" with linespoints pt 7 lc rgb "#CC2529" lw 1.5, \
	"BruteForceMultiThreadSpeedup5" title "5 cores" with linespoints pt 7 lc rgb "#535154" lw 1.5, \
	"BruteForceMultiThreadSpeedup6" title "6 cores" with linespoints pt 7 lc rgb "#6B4C9A" lw 1.5
unset output

set output "BFOSpeedup.tex"
plot [48:49152][*:12]"BruteForceMultiThreadSpeedup1" title "1 thread CPU" with linespoints pt 7 lc rgb "#396AB1" lw 1.5, \
	"BruteForceMultiThreadSpeedup6" title "6 threads CPU" with linespoints pt 7 lc rgb "#DA7C30" lw 1.5, \
	"BruteForceOffloadUniverseSpeedup" title "GPU Offload" with linespoints pt 7 lc rgb "#3E9651" lw 1.5
unset output

unset logscale

set xlabel "# cores"
set ylabel "effici\\\"entie"

set output "BHStrong.tex"
plot [][0:1.1] "BarnesHutMultiThreadStrong4096" title "4096 bodies" with linespoints pt 7 lc rgb "#396AB1" lw 1.5, \
	"BarnesHutMultiThreadStrong16384" title "16384 bodies" with linespoints pt 7 lc rgb "#DA7C30" lw 1.5, \
	"BarnesHutMultiThreadStrong65536" title "65536 bodies" with linespoints pt 7 lc rgb "#3E9651" lw 1.5, \
	"BarnesHutMultiThreadStrong262144" title "262144 bodies" with linespoints pt 7 lc rgb "#CC2529" lw 1.5, \
	"BarnesHutMultiThreadStrong1048576" title "1048576 bodies" with linespoints pt 7 lc rgb "#535154" lw 1.5
unset output

set output "BFStrong.tex"
plot [][0:1.1] "BruteForceMultiThreadStrong4096" title "4096 bodies" with linespoints pt 7 lc rgb "#396AB1" lw 1.5, \
	"BruteForceMultiThreadStrong8192" title "8192 bodies" with linespoints pt 7 lc rgb "#DA7C30" lw 1.5, \
	"BruteForceMultiThreadStrong16384" title "16384 bodies" with linespoints pt 7 lc rgb "#CC2529" lw 1.5, \
	"BruteForceMultiThreadStrong32768" title "32768 bodies" with linespoints
unset output

set term wxt

