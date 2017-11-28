-- ###################################################################
-- # Application : SQL script
-- # File        : create_vins.sql
-- # Revision    : aout 2015 (Marie Szafranski)
-- # Author      : Olivier Hubert (email: olivier.hubert@iie.cnam.fr)
-- # Function    : Create the structure of the VINS database
-- ###################################################################

-- If tables exist already
DROP TABLE livraison;
DROP TABLE commande;
DROP TABLE buveur;
DROP TABLE vin;
DROP TABLE viticulteur;

CREATE TABLE viticulteur(
n_viticulteur INTEGER,
nom VARCHAR(30),
prenom VARCHAR(20),
ville VARCHAR(30),
CONSTRAINT pk_viticulteur PRIMARY KEY(n_viticulteur));

CREATE TABLE vin( 
n_vin INTEGER,
cru VARCHAR(20) CONSTRAINT nn_cru NOT NULL,
millesime INTEGER CONSTRAINT nn_millesime NOT NULL,
region VARCHAR(15),
n_viticulteur INTEGER CONSTRAINT fk_nn_n_viticulteur NOT NULL,
CONSTRAINT pk_vin PRIMARY KEY(n_vin),
CONSTRAINT ck_vin UNIQUE(cru, millesime, n_viticulteur),
CONSTRAINT fk_viticulteur FOREIGN KEY (n_viticulteur) REFERENCES 
viticulteur(n_viticulteur), 
CONSTRAINT dom_millesime CHECK(millesime > 1970 AND millesime <
2012)); 

CREATE TABLE buveur( 
n_buveur INTEGER,
nom VARCHAR(20),
prenom VARCHAR(20),
ville VARCHAR(30),
CONSTRAINT pk_buveur PRIMARY KEY (n_buveur));

CREATE TABLE commande( 
n_commande INTEGER,
n_buveur INTEGER CONSTRAINT fk_nn_buveur NOT NULL,
n_vin INTEGER CONSTRAINT fk_nn_vin NOT NULL,
c_date DATE  CONSTRAINT nn_c_date NOT NULL,
c_qte INTEGER ,
CONSTRAINT pk_commande PRIMARY KEY (n_commande),
CONSTRAINT ck_commande UNIQUE (n_buveur, n_vin, c_date),
CONSTRAINT fk_buveur FOREIGN KEY(n_buveur) REFERENCES
buveur(n_buveur), 
CONSTRAINT fk_vin FOREIGN KEY(n_vin) REFERENCES vin(n_vin), 
CONSTRAINT dom_c_qte CHECK(c_qte =6 OR c_qte = 12 OR c_qte = 18)); 

CREATE TABLE livraison( 
n_commande INTEGER,
l_date DATE,
l_qte INTEGER CONSTRAINT nn_l_qte NOT NULL,
CONSTRAINT pk_livraison PRIMARY KEY(n_commande, l_date),
CONSTRAINT fk_commande FOREIGN KEY (n_commande) REFERENCES
commande(n_commande)); 

-- ALTER TABLE buveur RENAME ville TO b_ville;
-- ALTER TABLE viticulteur RENAME ville TO v_ville;

