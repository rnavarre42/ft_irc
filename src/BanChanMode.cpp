#include "BanChanMode.hpp"

void BanChanMode::loadChanMode(void)
{
			
}

void BanChanMode::unloadChanMode(void)
{
	//Tenemos que suscribirnos a JOIN, PRIVMSG y NOTICE
	//
	//ban acepta un parámetro, por lo que tenemos que comunicarselo
	//al servidor para que nos lo envíe.
}

void	BanChanMode::onChanEvent(Access &access, Message &message)
{
	//onChanEvent nos debe proporcionar un listado de las entradas
	//que coinciden con el modo que gestionamos, en esta ocasión 'b'
	//y verificar que la máscara de sender no está en el listado.
	//
	//en caso de que esté, se le deniega el comando, bien sea JOIN
	//PRIVMSG o NOTICE.
	//
	//lo único que cambiaría es el mensaje de error que es dstinto
	//en cada caso
	//
	//si se deniega, access = deny;
}

void	BanChanMode::onEnableChanModeEvent(Access &access, std::string &value)
{
	//TODO: value contendría la máscara a añadir
	//nesitamos comprobar si cumple con lo que sería una máscara
	//ver si no existe
	//agregarla

	//En este caso, el modo +b es una lista de máscaras que admiten
	//wildcards.
	//ntpsm no acepta parámetro alguno y por tanto value == empty()
	//
	//ov aceptan por parámetro un nick que exista en el canal.
	//Aquí se gregaría la instancia User de ese nick.
	//chanModeMultimap acepta un void * para que cada modo almacene
	//la información que necesite. Después, cada modo, se encargará
	//de volver a dejar ese void * al tipo de dato original.
	//
	//Si el parámetro no cumpliese, access = deny;
}

void	BanChanMode::onDisableChanModEvent(Access &access, std::string &value)
{
	//TODO: vaulec ontendría la máscara a eliminar
	//necesitamos comprobar si cumple con lo que sería una máscara
	//ver si no existe
	//borrarla

}

