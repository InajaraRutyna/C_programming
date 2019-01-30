/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: inajara
 *
 * Created on 07 de Outubro de 2016, 16:52
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{
    // numero de variaveis e p-medianas
    int n, i, j, k, d,wv;
    FILE *fp,*gnu,*data,*data1;
    srand(time(NULL));
    
    fp =fopen("resolucao.dat", "w"); 

    printf( "\n Número de Amostras \n");
    scanf( "%d", &n);
    
    printf( "\n Dimensão \n");
    scanf( "%d", &d);
    
    float x[n][d+1],erro,ta,erroa = 0.1;
    
    printf("\n Digite as amostras com os valores desejados\n");
    for(i = 0;i<n;i++){
        for(j = 0;j <= d;j++){
            scanf( "%f", &x[i][j]); 
        }
    }
    
    int it,neu,dim,ale,tam,cor[n];
    float thetab,alfa;
    tam = (int)x[0][0];
    for(i = 0;i<n;i++){
        for(j = 0;j <= d;j++){
            if (x[i][j] > tam)
                tam = (int)x[i][j]; 
        }
    }
    tam = tam + 1;
    printf( "\n Número de neurônios na camada escondida:\n");
    scanf( "%d", &neu);
   
    if (neu <= 0){
        wv = d;
        dim = 1;
    }else{
        wv = neu;
        dim = neu;
    }
    float v[d][dim],theta[dim],w[wv];
    printf( "\n Coeficientes aleatórios? s = 1/n =0:\n");
    scanf( "%d", &ale);
    if(ale == 0){
        printf( "\n Digite w:\n");
        for(j = 0;j < wv;j++){
            scanf( "%f", &w[j]);
        }
        if(neu>0){
        printf( "\n Digite v:\n");
        for(i = 0;i < d;i++){
            for(j = 0;j < dim;j++){
                scanf( "%f", &v[i][j]);
            }
        }
        printf( "\n Digite theta a:\n");
        for(j = 0;j < dim;j++){
            scanf( "%f", &theta[j]);
        }
        }
        printf( "\n Digite theta b:\n");
        scanf( "%f", &thetab);
    }else{
        printf( "\n w:\n");
        
        for(j = 0;j < wv;j++){
            w[j] = ((rand()%200)/100.0)-1.00;
            printf("w_%d = %f ",j+1,w[j]);
        }
        printf( "\n");
        if(neu>0){
        printf( "\n v:\n");
        for(i = 0;i < d;i++){
            for(j = 0;j < dim;j++){
                v[i][j] = ((rand()%200)/100.0)-1.00;
                printf("v_%d_%d = %f ",i+1,j+1,v[i][j]);
            }
            printf( "\n");
        }
        printf( "\n theta a:\n");
        for(j = 0;j < dim;j++){
            theta[j] = ((rand()%200)/100.0)-1.00;
            printf("theta_%d = %f ",j+1,theta[j]);
        }
         printf( "\n");
        }
        printf( "\n theta b:\n");
        thetab = ((rand()%200)/100.0)-1.00;
        printf("thetab = %f ",thetab);
         printf( "\n");
        
    }
    printf("\n Digite alfa:\n");
    scanf( "%f", &alfa);
    
    printf("\n Taxa de aprendizagem:\n");
    scanf( "%f", &ta);
    
    printf("\n Numero de iterações:\n");
    scanf( "%d", &it);
        
    float ys,y[n],acerto,auxt,auxth;
    int teste, po[n],aux,au,fun;
    char re;
    float zs,z[dim];
    
    printf("\n tangente hiperbólica (1) ou sigmoidal (0):\n");
    scanf( "%d", &fun);
  
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
            if(neu > 0){
                for(k = 0;k<neu;k++){
                    zs = theta[k];
                    for(l = 0;l<d;l++){
                        zs = zs +  v[l][k]*x[to][l];
                    }
                    fprintf(fp,"z*_%d = %f ",k+1,zs);
                    if(fun == 1){
                        z[k]=tanh(zs);
                    }else{
                        z[k]= 1/(1+exp(-zs));
                    }
                    fprintf(fp,"z_%d = %f \n",k+1,z[k]);
                }
                auxn = neu;
            }else{
                for(l = 0;l<d;l++){
                    z[l] = x[to][l];
                }
                auxn = d;
                
            }
            ys = thetab;
            for(k = 0;k<auxn;k++){
                ys = ys + z[k]*w[k];
            } 
            fprintf(fp,"y* = %f\n",ys);
            
            if(fun == 1){
                y[to] =tanh(ys);
            }else{
                y[to] = 1/(1+exp(-ys));
            }
            
            fprintf(fp,"y = %f\n",y[to]);
            
            auxt = alfa*y[to]*(1-y[to])*(x[to][d]-y[to]);
            thetab = thetab + auxt;
            fprintf(fp,"\ntheta = %f\n",thetab);
            for(k = 0;k<auxn;k++){
                w[k] = w[k] + auxt*z[k];
                fprintf(fp,"w_%d = %f ",k+1,w[k]);
            }
            if(neu > 0){
                for(k = 0;k<auxn;k++){
                    auxth = auxt*w[k]*z[k]*(1-z[k]);
                    theta[k] = theta[k] + auxth;
                    fprintf(fp,"\ntheta_a_%d = %f\n",k+1,theta[k]);
                    for(l = 0;l<d;l++){
                        v[l][k] = v[l][k] + auxth*x[to][l];
                        fprintf(fp,"v_%d_%d = %f ",l+1,k+1,v[l][k]);
            }
                }
                
                
            }
            fprintf(fp,"\n\n");
        }
        fprintf(fp,"Verificação da rede para o conjunto de truncamento\n");
        erro = 0;
        for(j = 0;j<n;j++){
            to = po[j];
            fprintf(fp,"\n x%d = (",to+1);
            for(k = 0;k<d;k++){
                fprintf(fp,"%f ",x[to][k]);
            }
            fprintf(fp,")\n");
            if(neu > 0){
                for(k = 0;k<neu;k++){
                    zs = theta[k];
                    for(l = 0;l<d;l++){
                        zs = zs +  v[l][k]*x[to][l];
                    }
                    fprintf(fp,"z*_%d = %f ",k+1,zs);
                    if(fun == 1){
                        z[k]=tanh(zs);
                    }else{
                        z[k]= 1/(1+exp(-zs));
                    }
                    fprintf(fp,"z_%d = %f \n",k+1,z[k]);
                }
                auxn = neu;
            }else{
                for(l = 0;l<d;l++){
                    z[l] = x[to][l];
                }
                auxn = d;
                
            }
            ys = thetab;
            for(k = 0;k<auxn;k++){
                ys = ys + z[k]*w[k];
            } 
            fprintf(fp,"y* = %f\n",ys);
            
            if(fun == 1){
                y[to] =tanh(ys);
                if(y[to]>=0){
                    cor[to] = 1;
                }else{
                    cor[to] = 2;
                }
            }else{
                y[to] = 1/(1+exp(-ys));
                if(y[to]>=0.5){
                    cor[to] = 1;
                }else{
                    cor[to] = 2;
                }
            }
            fprintf(fp,"y = %f\n",y[to]);
            erro = erro + (x[to][d] - y[to])*(x[to][d] - y[to]);
        }
        erro = erro/2;
        fprintf(fp,"Erro = %f\n",erro);
        fprintf(fp,"Fim da iteração %d \n ",i+1);
        if(erro<erroa){
        i = it;
        }else{
        i++;
        alfa = alfa*ta;
        }
        
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
        fprintf(fp,"Verificação do conjunto de testes\n");
        erro = 0;
        for(to = 0;to<n1;to++){
            if(neu > 0){
                float zs,z[neu];
                for(k = 0;k<neu;k++){
                    zs = theta[k];
                    for(l = 0;l<d;l++){
                        zs = zs +  v[l][k]*x[to][l];
                    }
                    fprintf(fp,"z*_%d = %f ",k+1,zs);
                    if(fun == 1){
                        z[k]=tanh(zs);
                    }else{
                        z[k]= 1/(1+exp(-zs));
                    }
                    fprintf(fp,"z_%d = %f \n",k+1,z[k]);
                }
                auxn = neu;
            }else{
                for(l = 0;l<d;l++){
                    z[l] = x[to][l];
                }
                auxn = d;
                
            }
            ys = thetab;
            for(k = 0;k<auxn;k++){
                ys = ys + z[k]*w[k];
            } 
            fprintf(fp,"y* = %f\n",ys);
            
            if(fun == 1){
                y[to] =tanh(ys);
                if(y[to]>=0){
                    cor[to] = 1;
                }else{
                    cor[to] = 2;
                }
            }else{
                y[to] = 1/(1+exp(-ys));
                if(y[to]>=0.5){
                    cor[to] = 1;
                }else{
                    cor[to] = 2;
                }
            }
            fprintf(fp,"y = %f\n",y[to]);
            erro = erro + (x[to][d] - y[to])*(x[to][d] - y[to]);
        }
        erro = erro/2;
        fprintf(fp,"Erro = %f\n",erro);
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