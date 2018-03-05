---
title: "TP2 stats"
author: "Romain PEREIRA"
date: "5 Mars 2018"
output:
  html_document:
    df_print: paged
  pdf_document:
    fig_caption: yes
---

# 1. Echantillon, Théorème Central Limite, Estimation Monte Carlo
### 1.1 Simulation de 1000 échantillon i.i.d gaussien.
```{r, include=TRUE}
N <- 1000

empirical_mean <- function(vec) {
  s = 0
  for (x in vec) {
    s = s + x
  }
  return (s / (length(vec) - 1))
}

empirical_var <- function(vec) {
  m = empirical_mean(vec)
  s = 0
  for (x in vec) {
    s = s + (x - m)
  }
  return (s / (length(vec) - 1))
}


mean_gauss <- function(n) {
  gauss <- matrix(rnorm(n * N, mean=1, sd=2), nrow=n, ncol=N)
  means <- 1 / (n - 1) * apply(gauss, 2, empirical_mean)
  hist(means, xlab="Moyenne empirique", main=paste("Distribution Gaussienne N(1, 2) avec n=", n))
}

mean_gauss(5)
mean_gauss(30)
mean_gauss(100)
```

D'après le théorème central limite, la loi théorique de la moyenne empirique est une loi normal $N(\mu=1, \sigma=2/\sqrt{n})$

En notant $(a_n, b_n) = (\mu, \frac{\sigma}{\sqrt{n}}) = (1, \frac{2}{\sqrt{n}})$,
$U_n = \frac{\bar{X_n} - a_n}{b_n}$ suit une loi normal centrée réduite $N(0, 1)$.

```{r, include=TRUE}
mean_gauss_normalized <- function(n) {
  gauss <- matrix(rnorm(n * N, mean=1, sd=2), nrow=n, ncol=N)
  an <- 1
  bn <- 2 / sqrt(n)
  means <- 1 / (n - 1) * apply(gauss, 2, function(x) (x - an) / bn)
  hist(means, xlab="Moyenne empirique", main=paste("N(1, 2) renormalisé en N(0, 1) avec n=", n))
}

mean_gauss_normalized(5)
mean_gauss_normalized(30)
mean_gauss_normalized(100)
```

Les histogrammes obtenus montrent en effet une loi normale centrée réduite.

Plus $n$ est grand, plus la loi moyenne empirique renormalisé semble suivre une loi N(0, 1).
(cf [Théorème Central Limite](https://fr.wikipedia.org/wiki/Théorème_central_limite))

### 1.2 Loi de Pareto

### 1.3 Loi de Poisson
