#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Library.hpp"
class Request;
class Config;
class Response
{
	public:

		Response();
		Response(Request request, Config config);
		Response(const Response &other);
		Response &operator=(const Response &other);
		~Response();

		std::string	getResponse() const;

	private:

		Location*	_location;

		std::string	_protocol;
		std::string	_code;
		std::string	_mime;
		std::string	_body;
		std::string _cgiResponse;

		std::string getExtension(std::string file);

		void	getCode(std::string code);
		void	getMime(std::string file);
		void	getErrorPage(Config config, std::string error);

		Location *getRequestLocation(Request request, Config config);
		std::string getPath(Request request, Config config);
		std::string getIndex(Config config);
		bool isAllowedMethod(std::string method);
		void getMethod(Request request, Config config);
		void uploadFile(std::string path, std::string field);
		void staticPost(Request request, Config config);
		void postMethod(Request request, Config config);
		void deleteMethod(Request, Config config);
};

#endif
