#!/bin/bash
#concatena evasões e gera tabeça de evasões por ano
i=2014;
while [ $i -lt 2019 ]; do
    sed -i '/FORMA_EVASAO/d' evasao-$i.csv;  #Exclui o cabeçalho de todos os aqruivos de evasoes
    cat evasao-$i.csv >> evasoes_totais.csv;
    cut -d, -f1 evasao-$i.csv | sort | uniq -c | awk -v var=$i '{N+=$1} END {print var, N}' >> evasoes_ano.txt #Calcula totalidade
    let i=i+1;
done;  

#imprime na tela o ranking  de evasões [ITEM 3]
cut -d, -f1  evasoes_totais.csv | sort | uniq -c | sort -nr > ranking_evasoes.txt; 
echo " ";echo "[ITEM 3]";echo " ";echo "Raking de evasões ao longo dos 5 anos:"
awk '{ print $2, $3, $4, $5, $6, $1, "evasões"}' ranking_evasoes.txt

#gera ranking de anos e imprime na tela o ranking de anos  [ITEM 4]
i=2014
while [ $i -lt 2019 ]; do
    sed -i '/ANO_INGRESSO/d' evasao-$i.csv; 
    cut -d, -f4 evasao-$i.csv | sort -nr > ano_de_ingresso_$i;
    cat ano_de_ingresso_$i | awk -v var=$i '{print $2, var - $1}' >> tmp.txt
    let i=i+1;
done; unset CONT;
rm ano_de_ingresso_201*; sort -n tmp.txt | uniq -c >> ranking_anos.txt; rm tmp.txt
echo " ";echo "[ITEM 4]";echo " ";echo "Raking de alunos ao longo dos 5 anos:"
awk '{print $1, "aluno(s)", $2, "ano(s)"}' ranking_anos.txt

#Gera porcentagem de semestre e imprime na tela [ITEM 5]
echo " ";echo "[ITEM 5]";echo " ";echo "Porcentagem de evasões de semestre ao longo dos 5 anos"
i=2014;
while [ $i -lt 2019 ]; do
    cut -d, -f2 evasao-$i.csv > tmp.txt;
    TOTAL=$(cut -d. -f2 tmp.txt| uniq -c | awk '{print $1}');
    sed "/1/d" tmp.txt > tmp2.txt;
    sed "/2/d" tmp.txt > tmp1.txt;
    cut -do -f1 tmp1.txt | uniq -c | sort -n > tmp.txt;
    SEME1=$(awk '{print$1}' tmp.txt);
    cut -do -f1 tmp2.txt | uniq -c | sort -n > tmp.txt;
    SEME2=$(awk '{print$1}' tmp.txt);
    TOTAL=$(echo "scale=5;($TOTAL/100)"|bc);
    if [ $SEME1 -gt $SEME2 ]; then      
        cat tmp1.txt| cut -do -f1 | uniq -c | awk -v var1=$i -v var2=$TOTAL '{print var1, $2"°Semestre", $1/var2"%"}'
    else
        cat tmp2.txt| cut -do -f1 | uniq -c | awk -v var1=$i -v var2=$TOTAL '{print var1, $2"°Semestre", $1/var2"%"}'
    fi
    let i=i+1;
done

#gera porcentagem de gênero em relações as evasões e imprime na tela [ITEM 6]
echo " ";echo "[ITEM 6]";echo " ";echo "Porcentagem de gênero em relação as evasões ao longo dos 5 anos:"
i=2014;
while [ $i -lt 2019 ]; do
    cut -d, -f5 evasao-$i.csv >> tmp.txt;
    let i=i+1;
done
sort tmp.txt | uniq -c >> tmp1.txt;
sed -i '/1/d' tmp1.txt; 
AUX=$(awk '{N+=$1} END {print N}' tmp1.txt)
AUX=$(echo "scale=5;($AUX/100)"|bc)
awk -v var=$AUX '{print $2, $1/var"%"}' tmp1.txt; rm tmp*

#Gera gráfico de evasoes por ano [ITEM 7]
gnuplot -e '
            reset;
            set terminal png size 900,600;
            set output "evasoes-ano.png";
            set title "Evasões por ano";
            set key outside right;
            set xlabel "Anos de evasoes";
            plot "evasoes_ano.txt" using ($1):($2) with lines smooth csplines t"Evasoes";
'
rm evasoes_ano.txt; #Exclui arquivo criado para manuntenção do código

#Gera tabela de forma de ingresso por ano
TIPOS=$(cat evasoes_totais.csv | cut -d, -f3 | sort | uniq -c | awk '{print $2$3$4$5$6}' | tr '\n' ' ');
ANO=2014;
while [ $ANO -lt 2019 ]; do        
    VETOR[0]=$ANO;
    i=1;
    for FORMA in $TIPOS; do
        QTD=$(cat evasao-$ANO.csv | cut -d, -f3 | sort | uniq -c | awk '{print $1, $2$3$4$5$6}' | grep "$FORMA" | awk '{print $1}')
        if [ -z $QTD ]; then
           QTD=0;
        fi
        VETOR[$i]=$QTD;
        i=$(($i+1));
    done;
    echo ${VETOR[@]} >> tabela_formas_de_ingresso.txt;
    let ANO=ANO+1;
done

#Gera de números de evasoões formas de ingresso [ITEM 8]
gnuplot <<- EOF
            set term png size 900,600;
            set output 'evasoes-forma.png';
            set yrange [0:130]
            set ytic  10
            set xtic format ""
            set style data histogram
            set style fill solid
            set boxwidth 0.5
            set xlabel 'Anos';
            set ylabel 'Números de evasoes';
            set key font ",12"
            set key right outside
            set title "Números de evasões por formas de ingresso/anos";
            plot 'tabela_formas_de_ingresso.txt' using 2:xtic(1) title "Aluno Intercâmbio", \
                                              '' using 3:xtic(1) title "Aproveitamento Curso Superior", \
                                              '' using 4:xtic(1) title "Convênio AUGM", \
                                              '' using 5:xtic(1) title "Convênio Pec-G", \
                                              '' using 6:xtic(1) title "Mobilidade Acadêmica", \
                                              '' using 7:xtic(1) title "Processo Seletivo/ENEM", \
                                              '' using 8:xtic(1) title "Reopção", \
                                              '' using 9:xtic(1) title "Transferência Ex-Ofício", \
                                              '' using 10:xtic(1) title "Transferência Provar", \
                                              '' using 11:xtic(1) title "Vestibular"                        
EOF
rm ranking*; rm tabela* ; rm evasoes_totais.csv #Exclui arquivos criados para manuntenção do código
