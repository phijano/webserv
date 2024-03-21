#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Library.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "Config.hpp"
# include "CgiHandler.hpp"
class Response
{
	public:
		Response();
		Response(Request& request, Config& config);
		Response(const Response& other);
		Response &operator=(const Response& other);
		~Response();

		std::string	getResponse() const;

	private:
		Location	_location;

		std::string	_protocol;
		std::string	_code;
		std::string	_mime;
		std::string	_body;
		std::string _cgiResponse;

		std::string getExtension(const std::string& file);

		void	setCode(const std::string& code);
		void	setMime(const std::string& file);
		void	getErrorPage(const Config& config, const std::string error);

		Location getRequestLocation(const Request& request, Config& config);
		std::string getPath(const Request& request, const Config& config);
		std::string getIndex(const Config& config);
		bool isAllowedMethod(const std::string& method);
		void getMethod(const Request& request, const Config& config);
		void uploadFile(const std::string& path, const std::string& formField);
		void staticPost(const Request& request, const Config& config);
		void postMethod(const Request& request, const Config& config);
		void deleteMethod(const Request& request, const Config& config);
};

#endif
