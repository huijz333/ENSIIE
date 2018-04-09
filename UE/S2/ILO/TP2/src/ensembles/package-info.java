/**
 * Package contenant la définition d'un {@link ensembles.Ensemble} comme étant
 * une collection (a priori non ordonnée, même si le conteneur sous jacent peut
 * être ordonné). {@link ensembles.EnsembleGenerique} fournit une implémentation
 * partielle des ensembles sans connaître encore le conteneur sous-jacent (qui
 * peut être un {@link java.util.Vector}, ou bien une {@link listes.Liste}, ou
 * encore un {@link tableaux.Tableau}. {@link ensembles.EnsembleGenerique}
 * n'implémente pas les opérations :
 * <ul>
 * <li>d'ajout {@link ensembles.EnsembleGenerique#ajout(Object)} puisqu'elle est
 * spécifique au conteneur sous-jacent</li>
 * <li>de construction d'un itérateur
 * {@link ensembles.EnsembleGenerique#iterator()} puisqu'elle est aussi
 * spécifique au conteneur sous-jacent</li>
 * <li>les opérations ensembliste comme
 * {@link ensembles.Ensemble#union(Ensemble)},
 * {@link ensembles.Ensemble#intersection(Ensemble)},
 * {@link ensembles.Ensemble#complement(Ensemble)} et
 * {@link ensembles.Ensemble#difference(Ensemble)} de part le fait qu'elle est
 * une classe abstraite et ne peut donc pas "créer" l'ensemble résultat de
 * l'opération ensembliste. En revanche elle propose une implémentation basée
 * sur les méthodes de classes dans lesquelle l'ensemble résultat est déjà créé
 * (par une des classes filles)</li>
 * </ul>
 * {@link ensembles.EnsembleGenerique} implémente donc
 * <ul>
 * <li>{@link ensembles.Ensemble#union(Ensemble, Ensemble, Ensemble)}</li>
 * <li>{@link ensembles.Ensemble#intersection(Ensemble, Ensemble, Ensemble)}</li>
 * <li>{@link ensembles.Ensemble#complement(Ensemble, Ensemble, Ensemble)}</li>
 * <li>{@link ensembles.Ensemble#difference(Ensemble, Ensemble, Ensemble)}</li>
 * </ul>
 */
package ensembles;