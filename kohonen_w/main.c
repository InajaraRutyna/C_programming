/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: inajara
 *
 * Created on 1 de Novembro de 2016, 14:09
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{
    // numero de variaveis e p-medianas
    int n, i, j, k, d,teste, it;
    FILE *fp,*gnu,*data,*data1;
    srand(time(NULL));
    
    fp =fopen("resolucao.dat", "w"); 

    printf( "\n Número de Amostras \n");
    scanf( "%d", &n);
    
    printf( "\n Dimensão \n");
    scanf( "%d", &d);
    
    float x[n][d+1],theta,ta,acerto,alfa;
    
    printf("\n Digite as amostras com os valores desejados\n");
    float min, max;
    for(i = 0;i<n;i++){
        for(j = 0;j <= d;j++){
            scanf( "%f", &x[i][j]); 
        }
    }
    char re;
    int m, nw;
    
    printf( "\n Número de neuronios \n");
    scanf( "%d", &m);
    float w[m][n], passo;
    
    printf( "\n Entrar com pesos iniciais? s/n \n");
    scanf( " %c", &re);
    if(re == 's'){
        for(i = 0;i<n;i++){
            for(j = 0;j <= d;j++){
                scanf( "%f", &w[i][j]); 
            }
        }
        
    }else{
        nw = m +1;
        passo = 0;
        for(i = 0;i<nw;i++){ 
            w[i][0] = passo;
            w[i][1] = 0;
            w[i+nw][0] = 1;
            w[i+nw][1] = passo;
            w[i+2*nw][0] = 1-passo;
            w[i+2*nw][1] = 1;
            w[i+3*nw][0] = 0;
            w[i+3*nw][1] = 1-passo;
            passo = passo + 1/nw;
        }
    }


    printf("\n alfa:\n");
    scanf( "%f", &alfa);
    
    printf("\n Taxa de aprendizagem:\n");
    scanf( "%f", &ta);
    
    printf("\n Numero de iterações:\n");
    scanf( "%d", &it);
        
    float ys,y[n];
    int po[n],aux,au;
    char re;

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
    float dis;
    while(i<it){
        fprintf(fp,"\nIteração %d \n ",i+1);
        for(j = 0;j<n;j++){
            to = j;
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
}

