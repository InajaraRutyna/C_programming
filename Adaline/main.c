/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: inajara
 *
 * Created on 29 de Setembro de 2016, 16:52
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{
    // numero de variaveis e p-medianas
    int n, i, j, k, d,bi = -1;
    FILE *fp,*gnu, *data;
    fp =fopen("resolucao.dat", "w"); 

    printf( "\n Número de Amostras \n");
    scanf( "%d", &n);
    
    printf( "\n Dimensão \n");
    scanf( "%d", &d);
    
    float x[n][d+1];
    
    printf("\n Digite as amostras com os valores desejados\n");
    for(i = 0;i<n;i++){
        for(j = 0;j <= d;j++){
            scanf( "%f", &x[i][j]); 
        }
    }
    
    int it;
    float w[d], theta, alfa,ta;
    srand(time(NULL));
    
    /*printf( "\n w:\n");
    for(j = 0;j < d;j++){
        w[j] = ((rand()%300)/100.0)-1.00;
        printf("w_%d = %f ",j+1,w[j]);
    }
    printf("\n");*/
    
    printf( "\n Digite w:\n");
    for(j = 0;j < d;j++){
        scanf( "%f", &w[j]);
    }
    
    printf( "\n Digite alfa:\n");
    scanf( "%f", &alfa);
    
    printf( "\n Taxa de aprendizagem:\n");
    scanf( "%f", &ta);
    
    printf( "\n theta = 0\n");
    theta = 0;
    
    printf("\n Numero de iterações:\n");
    scanf( "%d", &it);
    
    float ys[n], y[n],acerto;
    int teste, po[n],aux,au;
    char re;
  
    for(j = 0;j<n;j++){
        po[j] = j;
    }
    printf( "\n Reordenar amosta? s/n \n");
    scanf( " %c", &re);

    i = 0;
    int to;
    float erro;
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
            ys[to] = theta;
            fprintf(fp,"Apresenta para a rede: x%d = (",to);
            for(k = 0;k<d;k++){
                fprintf(fp,"%f ",x[to][k]);
                ys[to] = ys[to] + x[to][k]*w[k];
            }
            fprintf(fp,")\n");
            fprintf(fp,"y* = %f\n",ys[to]);
            for(k = 0;k<d;k++){
                w[k] = w[k] + (alfa*(x[to][d] - ys[to])*x[to][k]);
                fprintf(fp,"w_%d = %f ",k+1,w[k]);
            }
            theta = theta + alfa*(x[to][d] - ys[to]);
            fprintf(fp,"\ntheta = %f\n",theta);
            if(ys[to] >= 0){
                y[to] = 1;
            }else{
                    y[to] = 0;
            }
            fprintf(fp,"y = %f\n",y[to]);
            fprintf(fp,"\n");
        }
        fprintf(fp,"Verificação da rede para o conjunto de truncamento\n");
        teste = 0;
        erro = 0;
        for(j = 0;j<n;j++){
            ys[j] = theta;
            fprintf(fp,"x%d = (",j);
            for(k = 0;k<d;k++){
                fprintf(fp,"%2.f ",x[j][k]);
                ys[j] = ys[j] + x[j][k]*w[k];
            }
            erro = erro + pow(x[j][d]-ys[j],2);
            fprintf(fp,"): ");
            fprintf(fp,"y* = %f\n",ys[j]);
            if(ys[j] >=0){
                y[j] = 1;
            }else{
                y[j] = -1;
            }
            fprintf(fp,"y = %f ",y[j]);
            if(y[j]/fabs(y[j]) != x[j][d]/fabs(x[j][d])){
                fprintf(fp,"diferente de %f - errado\n\n",x[j][d]);
            }else{
                fprintf(fp,"igual a  %f - correto\n\n",x[j][d]);
                teste = teste + 1;
            }
        }
        acerto = teste*100/n;
        fprintf(fp,"Acerto = %f%%\n",acerto);
        erro = erro/2;
        fprintf(fp,"Erro = %f%%\n",erro);
        fprintf(fp,"Fim da iteração %d \n ",i+1);
        alfa = alfa*ta;
        if((teste == n)&&(erro<0.5)){
        i = it;
        }else{
        i++;
        }
    }
    
    
    int tam = 0;
    printf( "\n tamanho dos eixos x e y:\n");
    scanf( "%d", &tam);
    
    gnu = popen("gnuplot","w");
    fprintf(gnu, "set xrange [-%d:%d]\n",tam,tam);
    fprintf(gnu, "set yrange [-%d:%d]\n",tam,tam);
    fprintf(gnu, "set zrange [-1:1]\n");
    fprintf(gnu, "set tics scale 0\n");
    
    data =fopen("dados.dat", "w"); 
    
    for(j = 0;j<n;j++){
        fprintf(gnu,"set style arrow %d ls %d\n",j+1,3);
        fprintf(gnu,"set arrow %d from ",j+1);
        for(k = 0;k<d;k++){ 
            fprintf(data, "%f ", x[j][k]);
            fprintf(gnu,"%f, ",x[j][k]);
            }
        fprintf(data, "%f \n", ys[j]);
        fprintf(gnu,"0 to ");
        for(k = 0;k<d;k++){ 
            fprintf(data, "%f ", x[j][k]);
            fprintf(gnu,"%f, ",x[j][k]);
        }
        fprintf(data, " 0 \n");
        fprintf(gnu,"%f as %d\n",ys[j],j+1);
    }
    fclose(data);
    fprintf(gnu, "set grid ytics lt 0 lw 1 \n");
    fprintf(gnu,"set grid xtics lt 0 lw 1 \n");
    fprintf(gnu, "splot \"dados.dat\" using 1:2:3 w points pt 5,\n");
    fprintf(gnu, "set xyplane at 0\n");
    fprintf(gnu, "set term png \n");
    fprintf(gnu, "set output 'printme.png' \n");
    fprintf(gnu, "replot\n");
    fprintf(gnu, "set term x11\n");
    pclose(gnu);  
   
    char t;
    printf( "\n Testar valores? s/n \n");
    scanf( " %c", &t);
    if(t == 's'){
   
        printf( "\n Número de Amostras \n");
        scanf( "%d", &n);
    
        printf("\n Digite o conjunto de testes com os valores desejados\n");
        for(i = 0;i<n;i++){
            for(j = 0;j <= d;j++){
                scanf( "%f", &x[i][j]); 
            }
        }
        teste = 0;
        erro = 0;
        for(j = 0;j<n;j++){
            ys[j] = theta;
            fprintf(fp,"x%d = (",j);
            for(k = 0;k<d;k++){
                fprintf(fp,"%2.f ",x[j][k]);
                ys[j] = ys[j] + x[j][k]*w[k];
            }
            erro = erro + pow(x[j][d]-ys[j],2);
            fprintf(fp,"): ");
            fprintf(fp,"y* = %f\n",ys[j]);
            if(ys[j] >=0){
                y[j] = 1;
            }else{
                y[j] = -1;
            }
            fprintf(fp,"y = %f ",y[j]);
            if(y[j]/fabs(y[j]) != x[j][d]/fabs(x[j][d])){
                fprintf(fp,"diferente de %f - errado\n\n",x[j][d]);
            }else{
                fprintf(fp,"igual a  %f - correto\n\n",x[j][d]);
                teste = teste + 1;
            }
        }
        acerto = teste*100/n;
        fprintf(fp,"Acerto = %f%%\n",acerto);
        fprintf(fp,"Erro = %f%%\n",erro/2);
    }
    fclose(fp);
}