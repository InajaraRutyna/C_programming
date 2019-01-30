/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: inajara
 *
 * Created on 14 de Outubro de 2016, 13:26
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{

    int n, i, j, k,l, d,ce,tam,ale,aux,au;
    FILE *fp;
    srand(time(NULL));
    
    fp =fopen("resolucao.dat", "w"); 

    printf( "\n Número de Amostras \n");
    scanf( "%d", &n);
    
    printf( "\n Dimensão \n");
    scanf( "%d", &d);
    
    float x[n][d+1],theta,norma,raio,erro;
    
    printf("\n Digite as amostras com os valores desejados\n");
    for(i = 0;i<n;i++){
        for(j = 0;j <= d;j++){
            scanf( "%f", &x[i][j]); 
        }
    }
    tam = (int)x[0][0];
    for(i = 0;i<n;i++){
        for(j = 0;j <= d;j++){
            if (x[i][j] > tam)
                tam = (int)x[i][j]; 
        }
    }
    
    
    int po[n],to;
    
    printf( "\n Coeficientes aleatórios? s = 1/n =0:\n");
    scanf( "%d", &ale);
    
    printf( "\n Número de centros: \n");
    scanf( "%d", &ce);
    
    float c[ce][d],GG[ce+1][ce+ce+2],gd[ce+1],w[ce+1],g[n][ce+1];
    
    if(ale == 0){
        printf( "\n Entre com os centros: \n");
        for(i = 0;i<ce;i++){
            for(j = 0;j < d;j++){
                scanf( "%f", &c[i][j]); 
            }
        }
        printf( "\n Entre com o raio: \n");
        scanf( "%f", &raio);
        
        printf( "\n Digite theta:\n");
        scanf( "%f", &theta);
    }else{
        printf( "\n Centros: \n");
        for(j = 0;j<n;j++){
            po[j] = j;
        }
        for(j = 0;j<(n*2);j++){
            aux = (int)((rand()%100)%n);
            au = po[aux];
            po[aux] = po[j];
            po[j] = au;
        }
        for(i = 0;i<ce;i++){
            for(j = 0;j < d;j++){
                aux = po[i];
                c[i][j] = x[aux][j]; 
            }
        }
        printf( "\n Raio: \n");
        scanf( "%f", &raio);
        
        printf( "\n theta:\n");
        theta = ((rand()%200)/100.0)-1.00;
    }  
    
    for(j = 0;j<n;j++){
        po[j] = j;
    }
    char re;
    printf( "\n Reordenar amosta? s/n \n");
    scanf( " %c", &re);
    
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
        fprintf(fp,"\nApresenta para a rede: x%d = (",to+1);
        for(k = 0;k<d;k++){
            fprintf(fp,"%f ",x[to][k]);
        }
        fprintf(fp,")\n");
        fprintf(fp,"\nDistancias do x para os centros\n"); 
        for(k = 0;k<ce;k++){
            norma = 0;
            fprintf(fp,"||x%d - u%d||² = ",to+1,k+1);
            for(l = 0;l<ce;l++){
                norma = norma + pow(x[to][l]-c[k][l],2);
                fprintf(fp,"(%f - %f)² + ",x[to][l],c[k][l]);
            }
            fprintf(fp," = %f\n",norma);
            g[to][k] = exp((-norma)/(2*raio*raio));             
        }
        g[to][ce] = theta;
    }
    fprintf(fp,"\nMatriz G = ");
    for(k = 0;k<n;k++){
        for(j = 0;j<ce+1;j++){
            fprintf(fp,"%f ",g[k][j]);
        }
        fprintf(fp,"\n");
    }
    for(j = 0;j<ce+1;j++){
        for(k = 0;k<ce+1;k++){ 
            GG[j][k] = 0;
            for(l = 0;l<n;l++){
                GG[j][k] = GG[j][k] + g[l][j]*g[l][k];
            }
        }
        gd[j] = 0;
        for(l = 0;l<n;l++){
            gd[j] = gd[j] + g[l][j]*x[l][d];
        }
    }
    fprintf(fp,"\nMatriz GG = ");
    for(k = 0;k<ce+1;k++){
        for(j = 0;j<ce+1;j++){
            fprintf(fp,"%f ",GG[k][j]);
        }
        fprintf(fp,"\n");
    }
    float ratio,a,s = ce+1;
    for(i = 0; i < ce+1; i++){
        for(k = ce+1; k < 2*(ce+1); k++){
            if(i==(k-ce-1))
                GG[i][k] = 1.0;
            else
                GG[i][k] = 0.0;
        }
    }

    for(i = 0; i < s; i++){
        for(j = 0; j < s; j++){
            if(i!=j){
                ratio = GG[j][i]/GG[i][i];
                for(k = 0; k < 2*s; k++){
                    GG[j][k] -= ratio * GG[i][k];
                }
            }
        }
    }
    
    for(i = 0; i < s; i++){
        a = GG[i][i];
        for(j = 0; j < 2*s; j++){
            GG[i][j] /= a;
        }
    }
    fprintf(fp,"\nMatriz GG_inv = ");
    for(k = 0;k<ce+1;k++){
        for(j = ce+1; j < 2*(ce+1); j++){
            fprintf(fp,"%f ",GG[k][j]);
        }
        fprintf(fp,"\n");
    }
    for(j = 0;j<ce+1;j++){
        w[j] = 0;
        for(k = ce+1;k<2*(ce+1);k++){ 
            w[j] = w[j] + GG[j][k]*gd[k-ce-1];
        }
    }
        fprintf(fp,"\n");
    for(k = 0;k<ce+1;k++){
        fprintf(fp,"w_%d = %f ",k+1,w[k]);
    }
    fprintf(fp,"\n");
    
    
    fprintf(fp,"\nValidação da rede: \n");
    erro = 0;
    float y[n];
    for(j = 0;j<n;j++){
        y[j] = 0;
        fprintf(fp,"\n x%d = (",j+1);
        for(k = 0;k<d;k++){
            fprintf(fp,"%f ",x[j][k]);
        }
        fprintf(fp,")\n");
        for(l = 0;l<ce+1;l++){
            y[j] = y[j] + g[j][l]*w[l];
        }
        fprintf(fp,"y_%d = %f \n",j+1,y[j]);
        fprintf(fp,"erro_%d = %f \n",j+1,(pow(x[j][d] - y[j],2)/2));
        erro = erro + (pow(x[j][d] - y[j],2)/2);
    }
    erro = erro;
    fprintf(fp,"Erro = %f\n",erro);
    
    char t;
    printf( "\n Testar valores? s/n \n");
    scanf( " %c", &t);
    if(t == 's'){
        int n1;
        printf( "\n Número de Amostras \n");
        scanf( "%d", &n);
    
        printf("\n Digite o conjunto de testes com os valores desejados\n");
        for(i = 0;i<n;i++){
            for(j = 0;j <= d;j++){
                scanf( "%f", &x[i][j]); 
            }
        }
        fprintf(fp,"\nValidação do conjunto de teste: \n");
        erro = 0;
        float y[n];
        for(j = 0;j<n;j++){
            y[j] = 0;
            fprintf(fp,"\n x%d = (",j+1);
            for(k = 0;k<d;k++){
                fprintf(fp,"%f ",x[j][k]);
            }
            fprintf(fp,")\n");
            for(l = 0;l<ce+1;l++){
                y[j] = y[j] + g[j][l]*w[l];
            }
            fprintf(fp,"y_%d = %f \n",j+1,y[j]);
            fprintf(fp,"erro_%d = %f \n",j+1,(pow(x[j][d] - y[j],2)/2));
            erro = erro + (pow(x[j][d] - y[j],2)/2);
        }
        erro = erro;
        fprintf(fp,"Erro = %f\n",erro);
    }  
    fclose(fp);   
}
