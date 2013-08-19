queryImg="\"${${1##*/}%.dat}\""
retrSet=""
for i in $@
do
	if [ $i != $1 ]; then
		if [ $retrSet != "" ]; then
			retrSet="$retrSet,\n\t\t\"${${i##*/}%.mat}\""
		else
			retrSet="\t\t\"${${i##*/}%.mat}\""
		fi
	fi
done

echo "{\n\t\"query\": $queryImg,"
echo "\t\"dataset\": [\n $retrSet\n\t]\n}"
