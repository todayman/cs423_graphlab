#! /bin/sh

for dest in `cat ~/remote_machines`
do
	echo "Transfering to $dest"
	rsync -az ~/graphlabapi/apps/paul_kcore $dest:~/graphlabapi/apps/
	#echo "Building"
	#ssh $dest "cd graphlabapi; ./configure; cd debug/apps/paul_kcore; make; cd ../../../release/apps/paul_kcore; make;" &
done
