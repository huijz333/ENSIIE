DELETE FROM livraison;
DELETE FROM commande;
DELETE FROM buveur;
DELETE FROM vin;
DELETE FROM viticulteur;

\copy viticulteur(n_viticulteur, nom, prenom, ville) FROM 'fill_viticulteurs.csv' WITH DELIMITER AS ','
\copy vin(n_vin, cru, millesime, region, n_viticulteur) FROM 'fill_vins.csv' WITH DELIMITER AS ','
\copy buveur(n_buveur, nom, prenom, ville) FROM 'fill_buveurs.csv' WITH DELIMITER AS ','
\i fill_lmd_commandes.sql 
\i fill_lmd_livraisons.sql     

-- INSERT INTO buveur (n_buveur, nom, prenom, ville)
-- VALUES (1900, 'MOULIN', 'JEAN', 'MACON');
-- INSERT INTO buveur (n_buveur, nom, prenom, ville)
-- VALUES (1950, 'MOULIN', 'JEAN', 'PARIS');
