package ensembles;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

/**
 * Factory permettant de créer différents types d'ensembles utilisés dans les
 * tests
 * @author davidroussel
 */
public class EnsembleFactory<E>
{
	/**
	 * Obtention d'un nouvel ensemble d'après le type d'ensemble souhaité et un
	 * contenu (éventuel) à copier dans le nouvel ensemble
	 * @param typeEnsemble le type d'ensemble demandé: soit
	 * {@link ensembles.EnsembleVector}, soit
	 * {@link ensembles.EnsembleListe}, soit
	 * {@link ensembles.EnsembleTableau}
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
	@SuppressWarnings("unchecked")
	public static <E> Ensemble<E> getEnsemble(Class<? extends Ensemble<E>> typeEnsemble, Iterable<E> content)
			throws SecurityException, NoSuchMethodException, IllegalArgumentException, InstantiationException,
			IllegalAccessException, InvocationTargetException
	{
		Constructor<? extends Ensemble<E>> constructor = null;
		Class<?>[] argumentsTypes = null;
		Object[] arguments = null;
		Object instance = null;

		if (content == null)
		{
			argumentsTypes = new Class<?>[0];
			arguments = new Object[0];
		}
		else
		{
			argumentsTypes = new Class<?>[1];
			argumentsTypes[0] = Iterable.class;
			arguments = new Object[1];
			arguments[0] = content;
		}

		constructor = typeEnsemble.getConstructor(argumentsTypes);

		if (constructor != null)
		{
			instance = constructor.newInstance(arguments);
		}

		return (Ensemble<E>) instance;
	}
}
