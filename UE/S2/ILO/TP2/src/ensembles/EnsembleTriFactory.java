package ensembles;

import java.lang.reflect.InvocationTargetException;

/**
 * Factory permettant de créer différents types d'ensembles triés utilisés dans
 * les tests
 *
 * @author davidroussel
 */
public class EnsembleTriFactory<E extends Comparable<E>>
{
	/**
	 * Obtention d'un nouvel ensemble trié d'après le type d'ensemble souhaité
	 * et un contenu (éventuel) à copier dans le nouvel ensemble
	 * @param typeEnsemble le type d'ensemble demandé: soit
	 * {@link ensembles.EnsembleTriVector}, soit
	 * {@link ensembles.EnsembleTriVector2}, soit
	 * {@link ensembles.EnsembleTriListe}, soit
	 * {@link ensembles.EnsembleTriListe2}, soit
	 * {@link ensembles.EnsembleTriTableau}, soit
	 * {@link ensembles.EnsembleTriTableau2}
	 * @param content le contenu éventuel à copier dans le nouvel ensemble ( si
	 * celui ci est nul le constructeur par défaut sera appelé, s'il
	 * est non null, le constructeur de copie sera appelé
	 * @return une nouvelle instance de l'ensemble correspondant au type demandé
	 * @throws SecurityException Si le SecurityManager ne permet pas l'accès au
	 * constructeur demandé
	 * @throws NoSuchMethodException Si le constructeur demandé n'existe pas
	 * @throws IllegalArgumentException Si le nombre d'arguments fournis au
	 * constructeur n'est pas le bon
	 * @throws InstantiationException si la classe demandée est abstraite
	 * @throws IllegalAccessException Si le constructeur demandé est
	 * inaccessible
	 * @throws InvocationTargetException si le constructeur invoqué déclenche
	 * une exception
	 */
	public static <E extends Comparable<E>> EnsembleTri<E> getEnsemble(Class<? extends EnsembleTri<E>> typeEnsemble,
			Iterable<E> content) throws SecurityException, NoSuchMethodException, IllegalArgumentException,
					InstantiationException, IllegalAccessException, InvocationTargetException
	{
		return (EnsembleTri<E>) EnsembleFactory.<E> getEnsemble(typeEnsemble, content);
	}
}
