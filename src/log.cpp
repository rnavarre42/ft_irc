void	log(int flag, std::string msg)
{
	if (flag | DEBUG)
		std::cout << msg << std::endl;
}
