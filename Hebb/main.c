/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: inajara
 *
 * Created on 14 de Outubro de 2016, 13:46
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
/*

*/
int main()
{
    // numero de variaveis e p-medianas
    int n, i, j, k, d,teste;
    FILE *fp,*gnu,*data,*data1;
    srand(time(NULL));
    
    fp =fopen("resolucao.dat", "w"); 

    printf( "\n Número de Amostras \n");
    scanf( "%d", &n);
    
    printf( "\n Dimensão \n");
    scanf( "%d", &d);
    
    float x[n][d+1],theta,ta,acerto,w[n],alfa;
    
    printf("\n Digite as amostras com os valores desejados\n");
    float min, max;
    min = 100;
    max = -100;
    for(i = 0;i<n;i++){
        for(j = 0;j <= d;j++){
            scanf( "%f", &x[i][j]); 
            if(j != d){
                if(x[i][j] < min){
                    min = x[i][j];
                }
                if(x[i][j] > max){
                    max = x[i][j];
                }
            }
        }
    }
    
    int it,tam,cor[n];
    tam = (int)x[0][0];
    for(i = 0;i<n;i++){
        for(j = 0;j <= d;j++){
            if (x[i][j] > tam)
                tam = (int)x[i][j]; 
        }
    }
    tam = tam + 1;
    
    printf( "\n theta: 0\n");
    for(j = 0;j < d;j++){
        w[j] = 0;
        printf("w_%d = %f ",j+1,w[j]);
    }
    printf( "\n");
    
    printf("\n alfa:\n");
    scanf( "%f", &alfa);
    
    printf("\n Taxa de aprendizagem:\n");
    scanf( "%f", &ta);
    
    printf("\n Numero de iterações:\n");
    scanf( "%d", &it);
        
    float ys,y[n];
    int po[n],aux,au;
    char re;
  
    for(j = 0;j<n;j++){
        po[j] = j;
    }
    printf( "\n Reordenar amosta? s/n \n");
    scanf( " %c", &re);
    srand(time(NULL));
    i = 0;
    int to, auxn,l;
    
    data =fopen("dados.dat", "w"); 
    for(j = 0;j<n;j++){
        for(k = 0;k<d;k++){ 
            fprintf(data, "%f ", x[j][k]);
        }
        fprintf(data, " 0 \n");
    }
    fclose(data);
    fprintf(fp,"x atualizado: \n");
    for(j = 0;j<n;j++){
        for(k = 0;k<d;k++){ 
            x[j][k] = (2*(x[j][k]-min)/(max-min))-1;
            fprintf(fp,"%f ",x[j][k]);
        }
        fprintf(fp,"\n");
    }
    
    gnu = popen("gnuplot","w");
    while(i<it){
        fprintf(fp,"\nIteração %d \n ",i+1);
        if(re == 's'){
            for(j = 0;j<n;j++){
                aux = (int)((rand()%100)%n);
                au = po[aux];
                po[aux] = po[j];
                po[j] = au;
            }
        }
        for(j = 0;j<n;j++){
            to = po[j];
            fprintf(fp,"Apresenta para a rede: x%d = (",to+1);
            for(k = 0;k<d;k++){
                fprintf(fp,"%f ",x[to][k]);
            }
            fprintf(fp,")\n");
            
            for(k = 0;k<d;k++){
                w[k] = w[k] + alfa*x[to][k]*x[to][d];
                fprintf(fp,"w_%d = %f ",k+1,w[k]);
            }
            
            theta = theta + alfa*x[to][d];
            fprintf(fp,"\ntheta = %f\n",theta); 
        }
        fprintf(fp,"\n\n");
        
        fprintf(fp,"Verificação da rede para o conjunto de truncamento\n");
        teste = 0;
        for(j = 0;j<n;j++){
            to = po[j];
            fprintf(fp,"\n x%d = (",to+1);
            for(k = 0;k<d;k++){
                fprintf(fp,"%f ",x[to][k]);
            }
            fprintf(fp,")\n");

            ys = theta;
            for(k = 0;k<d;k++){
                ys = ys + x[to][k]*w[k];
            } 
            fprintf(fp,"y* = %f\n",ys);
            
            if(ys < 0){
                y[to] = -1;
                cor[to] = 1;
            }else{
                y[to] = 1;
                cor[to] = 2;
            }
            fprintf(fp,"y = %f\n",y[to]);
            if(y[j] != x[j][d]){
                fprintf(fp,"diferente de %f - errado\n\n",x[j][d]);
            }else{
                fprintf(fp,"igual a  %f - correto\n\n",x[j][d]);
                teste = teste + 1;
            }
        }
        acerto = teste*100/n;
        fprintf(fp,"Acerto = %f%%\n",acerto);
        fprintf(fp,"Fim da iteração %d \n ",i+1);
        
        
        fprintf(gnu, "set xrange [-%d:%d]\n",tam,tam);
        fprintf(gnu, "set yrange [-%d:%d]\n",tam,tam);
        fprintf(gnu, "set zrange [-1:1]\n");
        fprintf(gnu, "set tics scale 0\n");

        for(j = 0;j<n;j++){
            fprintf(gnu,"set style arrow %d ls %d lw 2 linecolor %d\n",j+1,3,cor[j]);
            fprintf(gnu,"set arrow %d from ",j+1);
            for(k = 0;k<d;k++){ 
                fprintf(gnu,"%f, ",x[j][k]);
            }
            fprintf(gnu,"0 to ");
            for(k = 0;k<d;k++){ 
                fprintf(gnu,"%f, ",x[j][k]);
            }
            fprintf(gnu,"%f as %d\n",y[j],j+1);
        }
  
        fprintf(gnu, "set grid ytics lt 0 lw 1 \n");
        fprintf(gnu,"set grid xtics lt 0 lw 1 \n");
        fprintf(gnu, "splot \"dados.dat\" using 1:2:3 w points pt 5,\n");
        fprintf(gnu, "set xyplane at 0\n");
        fprintf(gnu, "set term png \n");
        fprintf(gnu, "set output 'it%d_.png' \n",i);
        fprintf(gnu, "replot\n");
        fprintf(gnu, "set term x11\n"); 
        fprintf(gnu, "reset\n");
        if(teste == n){
        i = it;
        }else{
        i++;
        alfa = alfa*ta;
        }
    }
    char t;
    printf( "\n Testar valores? s/n \n");
    scanf( " %c", &t);
    if(t == 's'){
        int n1;
        printf( "\n Número de Amostras \n");
        scanf( "%d", &n1);
    
        printf("\n Digite o conjunto de testes com os valores desejados\n");
        for(i = 0;i<n1;i++){
            for(j = 0;j <= d;j++){
                scanf( "%f", &x[i][j]); 
            }
        }
        tam = (int)x[0][0];
        for(i = 0;i<n1;i++){
            for(j = 0;j <= d;j++){
                if (x[i][j] > tam)
                    tam = (int)x[i][j]; 
            }
        }
        tam = tam + 1;
        fprintf(fp,"x teste atualizado: \n");
        for(j = 0;j<n1;j++){
            for(k = 0;k<d;k++){ 
                x[j][k] = (2*(x[j][k]-min)/(max-min))-1;
                fprintf(fp,"%f ",x[j][k]);
            }
            fprintf(fp,"\n");
        }
        fprintf(fp,"Verificação do conjunto de testes\n");
        teste = 0;
        for(j = 0;j<n1;j++){
            to = po[j];
            fprintf(fp,"\n x%d = (",to+1);
            for(k = 0;k<d;k++){
                fprintf(fp,"%f ",x[to][k]);
            }
            fprintf(fp,")\n");

            ys = theta;
            for(k = 0;k<d;k++){
                ys = ys + x[to][k]*w[k];
            } 
            fprintf(fp,"y* = %f\n",ys);
            
            if(ys < 0){
                y[to] = -1;
                cor[to] = 1;
            }else{
                y[to] = 1;
                cor[to] = 2;
            }
            fprintf(fp,"y = %f\n",y[to]);
            if(y[j] != x[j][d]){
                fprintf(fp,"diferente de %f - errado\n\n",x[j][d]);
            }else{
                fprintf(fp,"igual a  %f - correto\n\n",x[j][d]);
                teste = teste + 1;
            }
        }
        acerto = teste*100/n1;
        fprintf(fp,"Acerto = %f%%\n",acerto);
        fclose(fp);
        data1 =fopen("dados1.dat", "w"); 
        for(j = 0;j<n1;j++){
        for(k = 0;k<d;k++){ 
            fprintf(data1, "%f ", x[j][k]);
        }
        fprintf(data1, " 0 \n");
    }
    fclose(data1);
    
    for(j = 0;j<n1;j++){
        fprintf(gnu,"set style arrow %d ls %d lw 2 linecolor %d\n",j+1,3,cor[j]);
        fprintf(gnu,"set arrow %d from ",j+1);
        for(k = 0;k<d;k++){ 
            fprintf(gnu,"%f, ",x[j][k]);
        }
        fprintf(gnu,"0 to ");
        for(k = 0;k<d;k++){ 
            fprintf(gnu,"%f, ",x[j][k]);
        }
        fprintf(gnu,"%f as %d\n",y[j],j+1);
    }
    fprintf(gnu, "set xrange [-%d:%d]\n",tam,tam);
    fprintf(gnu, "set yrange [-%d:%d]\n",tam,tam);
    fprintf(gnu, "set zrange [-1:1]\n");
    fprintf(gnu, "set tics scale 0\n");
    fprintf(gnu, "set grid ytics lt 0 lw 1 \n");
    fprintf(gnu,"set grid xtics lt 0 lw 1 \n");
    fprintf(gnu, "splot \"dados1.dat\" using 1:2:3 w points pt 5,\n");
    fprintf(gnu, "set xyplane at 0\n");
    fprintf(gnu, "set term png \n");
    fprintf(gnu, "set output 'teste_.png' \n");
    fprintf(gnu, "replot\n");
    fprintf(gnu, "set term x11\n"); 
    fprintf(gnu, "reset\n");
    }
    pclose(gnu); 
}