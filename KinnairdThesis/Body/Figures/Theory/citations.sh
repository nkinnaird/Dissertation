#!/bin/bash

doit() {
rm -rf pub*.*
# argument is the spires record ID. Google publication, get DOI and then put DOI in spires and then click on cited link to get record ID.
# This returns "citeable" defined by Spires as publised or arxiv.
rec=1
#wget "http://inspirehep.net/search?ln=en&of=hx&rg=100&jrec="1"&p=refersto%3Arecid%3A"$1"+AND+collection%3Aciteable" -O pub_$rec.txt -o pub.log

wget "http://inspirehep.net/search?ln=en&of=hx&rg=100&jrec="1"&p=refersto%3Arecid%3A"$1"" -O pub_$rec.txt -o pub.log

NUM=`grep "</strong> records found" pub_1.txt | head -1 | sed s/strong//g | sed s/\<//g | sed s/\>//g  | awk '{print $1}'  | sed 's/\///g' | sed 's/,//g'` 
echo "TOTAL = "$NUM
while [ $rec -le $NUM ] ; do
 echo "Record "$rec" for pub "$1
#wget "http://inspirehep.net/search?ln=en&of=hx&rg=100&jrec="$rec"&p=refersto%3Arecid%3A"$1"+AND+collection%3Aciteable" -O pub_$rec.txt -o pub.log

wget "http://inspirehep.net/search?ln=en&of=hx&rg=100&jrec="$rec"&p=refersto%3Arecid%3A"$1"" -O pub_$rec.txt -o pub.log
 rec=$((rec+100))

done
cat pub_*.txt > pub.txt
sed s/\"//g pub.txt > pub2.txt
#
start_year=1999
end_year=`date +%Y`
# Add extra zero if add another publication
declare -a num_pub=(0 0 0 0 0 0);
declare -a num_tot=(0 0 0 0 0 0);
#echo "Test : "${num_pub[0]}" "${num_pub[1]}
totx=0
year=$start_year
while [ $year -le $end_year ] ; do
# Aug-2015: replaced @article search with @article\|@phd\|@book\|@inpro"
    num_pub[$2]=`grep -i "@article\|@phd\|@book\|@inpro" pub2.txt | cut -d : -f2 | awk '{print substr($0,0,4)}' | grep $year | wc -l`
#    opfile="debug_"$year"_"$2".txt"
#    cp pub2.txt $opfile
# num_pub[$2]=`grep -i "year           = "$year  pub2.txt | wc -l`
 i=0 
 tot=0
 while [ $i -lt $3 ] ; do 
   k=$i
   i=$((i+1))
   j=$((i+2))
   num_tot[$k]=`grep $year count_$year.txt | awk -v var=$j '{print $var}'`
   if [ $2 -eq $k ] ; then
       num=`echo "${num_pub[$2]} + ${num_tot[$k]}" | bc`
       num_tot[$k]=$num
   fi
   tot=`echo "$tot + ${num_tot[$k]}" | bc`
  done
# Put the totals back into the file (update if increase num of pubs)
 echo $year" "$tot" "${num_tot[0]}" "${num_tot[1]}" "${num_tot[2]}" "${num_tot[3]}" "${num_tot[4]}" "${num_tot[5]} > count_$year.txt 
 echo $year" "$tot" "${num_tot[0]}" "${num_tot[1]}" "${num_tot[2]}" "${num_tot[3]}" "${num_tot[4]}" "${num_tot[5]}
 totx=`echo "$totx + $tot" | bc`
 year=$((year+1))
 echo " Total = "$totx
done
}

make_count() {
rm -rf count*.txt
start_year=1999
end_year=`date +%Y`
year=$start_year
while [ $year -le $end_year ] ; do
# Add extra zero if add another publication (1st is total and then one for each publication)
 echo $year"  0 0 0 0 0 0 0" >> count_$year.txt 
 year=$((year+1))
done

# ---First column is spires record ID, 2nd is an index (1+#publication), 3rd total number of pubs  ---------------------
#
#  101:Phys.Rev.Lett. 82 (1999) 1632-1635; DOI=10.1103/PhysRevLett.82.1632
doit 500172 0 6
# 101: Phys. Rev. D 62, 091101(R) (2000); DOI=10.1103/PhysRevD.62.091101
doit 533384 1 6
# 587: Phys. Rev. Lett. 86, 2227–2231 (2001), DOI=10.1103/PhysRevLett.86.2227
doit 552899 2 6
# 403: Phys. Rev. Lett. 89, 101804 (2002), DOI=10.1103/PhysRevLett.89.101804
doit 591756 3 6
# 447: Phys. Rev. Lett. 92, 161802 (2004); DOI=10.1103/PhysRevLett.92.161802
doit 642522 4 6
# 572: Phys. Rev. D 73, 072003 (2006); DOI=10.1103/PhysRevD.73.072003     
doit 710962 5 6
# ---------------------------------------
cat count_*.txt > count.txt
rm -rf count_*.txt
rm -rf pub*.txt
}


#  -------------- create a .png using root ---------------------------------
do_root() {
cat > plot.C <<EOF

TGraph* doit(int np, float* year, float* pub, int icol) {
     TGraph *tpub= new TGraph(np,year,pub);
     tpub->SetTitle();
     tpub->SetMinimum(0.0);
     tpub->SetMaximum(350.0);
     tpub->SetMarkerColor(icol);
     tpub->SetLineColor(icol);
     tpub->SetLineWidth(2);
     tpub->SetMarkerStyle(20);
     tpub->SetMarkerSize(1.2);
     tpub->SetFillColor(icol);
     return tpub;
}

void draw_text(TLatex* dtx,int icol) {
   dtx->SetTextColor(icol);
   dtx->SetTextFont(42);
   dtx->SetTextSize(0.04);
   dtx->SetTextAlign(12);
   dtx->Draw();
}

   


void StyleSetup() {

    gROOT->SetStyle("Plain");
    gROOT->ForceStyle();

    gStyle->SetCanvasColor(0);
    // Pad spacing
    gStyle->SetPadTopMargin(.1);
    gStyle->SetPadBottomMargin(.15);

    gStyle->SetPadLeftMargin(.15);
    gStyle->SetPadRightMargin(.1);
    gStyle->SetPadBorderMode(0);

    gStyle->SetTitleYOffset(1.0);
    gStyle->SetTitleXOffset(1.0);
   
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);

    // Data marker blue
    gStyle->SetMarkerStyle(21);
    gStyle->SetMarkerSize(0.8);
    gStyle->SetMarkerColor(kBlue);

    // Font : arial
    gStyle->SetTextFont(42);
    gStyle->SetLabelFont(42,"X");
    gStyle->SetLabelFont(42,"Y");
    gStyle->SetTitleFont(42,"X");
    gStyle->SetTitleFont(42,"Y");

    gStyle->SetTitleXSize(0.03);
    gStyle->SetTitleYSize(0.03);

    gStyle->SetOptTitle(0);
    gStyle->SetHistLineWidth(2.0);

    gStyle->SetNdivisions(505,"X");
    gStyle->SetNdivisions(505,"Y");

    gStyle->SetLabelSize(0.040,"X");
    gStyle->SetLabelSize(0.040,"Y");

    gStyle->SetBarWidth(0.8);
}

void plot(int start_year,int end_year) {

  StyleSetup(); 
  TCanvas *c1 = new TCanvas ("c1","c1", 0,0,800,430);
  c1->Range(start_year-4,-130.0,end_year+3,600.0);

   FILE *fp = fopen("count.txt","r");
   if(!fp) {
      cout << "ERROR READING FILE count.txt - Exiting...";
      exit;
   }

   float year[100],total[100],p1[100],p2[100],p3[100],p4[100],p5[100],p6[100];
   int num_points = 0;
   int num_cite = 0;
   while(!feof(fp)) {
     float _year,_total,_p1,_p2,_p3,_p4,_p5,_p6;
     int lines = fscanf(fp, "%f %f %f %f %f %f %f %f", &_year,&_total,&_p1,&_p2,&_p3,&_p4,&_p5,&_p6);
      if (num_points < (end_year-start_year+1)) {
        year[num_points] = _year;
        total[num_points] = _total;
        p1[num_points] = _p1;
        p2[num_points] = _p1+_p2;
        p3[num_points] = _p1+_p2+_p3;
        p4[num_points] = _p1+_p2+_p3+_p4;
        p5[num_points] = _p1+_p2+_p3+_p4+_p5;
        p6[num_points] = _p1+_p2+_p3+_p4+_p5 + _p6;
        num_cite = num_cite  + _total;
        cout << " year = " << _year << " num_points = " << num_points << " _total = " << _total << " num_cite = " << num_cite << endl;
        num_points++;
      }
   }

   int icol = 2; 
   TMultiGraph *tall = new TMultiGraph();
// Phys.Rev.Lett. 82 1632-1635 (1999) 
// Phys. Rev. D 62, 091101(R) (2000)
// Phys. Rev. Lett. 86, 2227–2231 (2001)
// Phys. Rev. Lett. 89, 101804 (2002)
// Phys. Rev. Lett. 92, 161802 (2004)
// Phys. Rev. D 73, 072003 (2006)


   float ymax=430; // 350 for "citeable"

   TGraph* g1 = doit(num_points,year,p6,2);
   tall->Add(g1);
   float xpos=end_year-2.0;
   float ydif=20;
   TLatex *dt1 = new TLatex(xpos,ymax-30,"PRD 2006");
   
   TGraph* g2 = doit(num_points,year,p5,4);
   tall->Add(g2);
   TLatex *dt2 = new TLatex(xpos,ymax-30-1.*ydif,"PRL 2004");
 
  TGraph* g3 = doit(num_points,year,p4,3);
   tall->Add(g3);
   TLatex *dt3 = new TLatex(xpos,ymax-30-2.*ydif,"PRL 2002");

   TGraph* g4 = doit(num_points,year,p3,1);
   tall->Add(g4);
   TLatex *dt4 = new TLatex(xpos,ymax-30-3.*ydif,"PRL 2001");

   TGraph* g5 = doit(num_points,year,p2,6);
   tall->Add(g5);
   TLatex *dt5 = new TLatex(xpos,ymax-30-4.*ydif,"PRD 2000");

   TGraph* g6 = doit(num_points,year,p1,7);
   tall->Add(g6);
   TLatex *dt6 = new TLatex(xpos,ymax-30-5.*ydif,"PRL 1999");

   tall->SetMaximum(ymax);
   tall->Draw("AB");

   tall->GetXaxis()->SetTitle("");
   tall->GetYaxis()->SetTitle("# Citations per year");

   tall->GetXaxis()->SetLabelSize(0.06);
   tall->GetXaxis()->SetTitleSize(0.06);
   tall->GetXaxis()->SetLabelOffset(1.02); // move below ploy

   tall->GetYaxis()->SetLabelSize(0.06);
   tall->GetYaxis()->SetTitleSize(0.06);
   tall->GetYaxis()->SetLabelOffset(0.02);

   char dt_text[200];
   sprintf(dt_text,"BNL E821: %4i total citations",num_cite);
   TLatex *dt = new TLatex(start_year,ymax-45,dt_text);
   dt->SetTextFont(42);
   dt->SetTextSize(0.08);
   dt->SetTextAlign(12);
   dt->Draw();
   
   draw_text(dt1,2);
   draw_text(dt2,4);
   draw_text(dt3,3);
   draw_text(dt4,1);
   draw_text(dt5,6);
   draw_text(dt6,7);

   // Draw some tick marks & axis labels and x-axis again
   for (float xx = start_year; xx <= end_year; xx++) {
     TLine* tl1 = new TLine(xx,0.0,xx,6.0);
     tl1->Draw();
     TLine* tl2 = new TLine(start_year-0.5,0.0,end_year+0.5,0);
     tl2->Draw();
     char al_text[20];
     sprintf(al_text,"%4i",int(xx));
     TLatex *al = new TLatex(xx,-15.0,al_text);
     al->SetTextColor(1);
     al->SetTextFont(42);
     al->SetTextSize(0.05);
     al->SetTextAlign(12);
     al->SetTextAngle(315);
     al->Draw();

   }

 
   c1->Print("citations.png");
   exit;

}

EOF
}
make_count
do_root
start_year=1999
end_year=`date +%Y`
root -b -q "plot.C($start_year,$end_year)"
