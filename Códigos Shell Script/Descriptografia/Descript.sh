
NUM=0; 
for i in p a b c d e f g h i; do
  sed -i "s/$i/$NUM/g" $1;
  NUM=$(($NUM+1));
done;

PADROES=$(cat $1 | cut -d '=' -f2| tr 'j' ' ' | tr ')' ' ' | tr ' ' '\n' | sort | uniq -c | sort -nr| column -t|sed 's/   */:/g'| cut -d ':' -f2)
echo $PADROES;
for n in $PADROES; do
        echo $n;
        l=$(printf "\x$(printf %x $n)\n");
        echo $l;
        sed -i "s/$n/$l/g" $1;
done;
sed -i 's\47\/\g' $1;
sed -i 's\j\\g' $1;

