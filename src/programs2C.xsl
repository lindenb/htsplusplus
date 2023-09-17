<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:j="http://www.ibm.com/xmlns/prod/2009/jsonx"
	version="1.0"
	>
<xsl:output method="text" indent="yes"/>

<xsl:template match="/">
/**

THIS FILE WAS AUTO-GENERATED, DO NOT EDIT

*/
#ifndef PROGRAMS_ARGS_HH
#define PROGRAMS_ARGS_HH
#include &lt;cstdio&gt;
#include &lt;cstdlib&gt;
#include &lt;cstring&gt;
#include &lt;iostream&gt;
#include &lt;sstream&gt;
#include &lt;cctype&gt;
#include &lt;string&gt;
#include &lt;vector&gt;
#include &lt;getopt.h&gt;
#include "git.hh"

<xsl:apply-templates select="programs"/>

#endif
</xsl:template>


<xsl:template match="programs">

#define THROW_PROGRAM_ERROR(...) std::ostringstream os; os &lt;&lt;  "[" &lt;&lt; __FILE__ &lt;&lt; ":" &lt;&lt; __LINE__ &lt;&lt; program_name()  &lt;&lt;  "]" &lt;&lt; __VA_ARGS__; throw std::runtime_error(os.str());

class ProgramArgs {
	public:
		virtual const char* program_name()=0;
	protected:
		int32_t parseInt(const char* s) {
			return  std::stoi(s);
			}
		char parseChar(const char* s) {
			if(std::strcmp(s,"\\t")==0) return '\t';
			if(std::strcmp(s,"\\n")==0) return '\n';
			if(std::strcmp(s,"\\r")==0) return '\r';
			if(std::strlen(s)!=1) {
				THROW_PROGRAM_ERROR("Expected only one character");
				}
			return s[0];
			}
	public:
		/** files after parsing arguments */
		std::vector&lt;std::string&gt; program_files;

		ProgramArgs() {
			}
		virtual ~ProgramArgs() {
			}
		virtual std::ostream&amp; usage(std::ostream&amp; out)=0;
		virtual const char* program_version()=0;
		virtual bool parse(int argc,char** argv)=0;

		const char* oneFileOrNull() {
			if(this-&gt;program_files.empty()) return NULL;
			if(program_files.size()==1) return program_files[0].c_str();
			THROW_PROGRAM_ERROR("expected zero or one arguments but got (" &lt;&lt; program_files.size() &lt;&lt;")");
			}
		const char* oneAndOnlyOneFile() {
			if(program_files.size()==1) return program_files[0].c_str();
			THROW_PROGRAM_ERROR("expected one and only one arguments but got (" &lt;&lt; program_files.size() &lt;&lt;")");
			}
	};

<xsl:apply-templates select="program"/>


class MainArgs {
	public:
		virtual const char* program_name() {
			return "main";
			}
		virtual const char* program_version() {
			<xsl:choose>
				<xsl:when test="@version">return "<xsl:value-of select="@version"/>";</xsl:when>
				<xsl:otherwise>return "0.0.0";</xsl:otherwise>
			</xsl:choose>
			}
		virtual bool parse(int argc,char** argv) {
			return true;
			}

		virtual std::ostream&amp; usage(std::ostream&amp; out) {
	                out &lt;&lt; program_name() &lt;&lt; std::endl;
			out &lt;&lt; "Compilation: " &lt;&lt; __DATE__ &lt;&lt; " at " &lt;&lt; __TIME__ &lt;&lt; std::endl;
			out &lt;&lt; "git: " &lt;&lt; GIT_VERSION &lt;&lt; std::endl;
			out &lt;&lt; "Author: Pierre Lindenbaum PhD" &lt;&lt; std::endl;
			out &lt;&lt; "Programs: " &lt;&lt; std::endl;


			<xsl:for-each select="program[not(@hidden='false')]">
				<xsl:sort select="@name"/>
				out &lt;&lt; "\t<xsl:value-of select="@name"/>\t";
				<xsl:choose>
					<xsl:when test="short-description">
						<xsl:text>out &lt;&lt; "</xsl:text>
						<xsl:call-template name="escapeC">
							<xsl:with-param name="s" select="short-description/text()"/>
						</xsl:call-template>
						<xsl:text>";</xsl:text>
					</xsl:when>
					<xsl:when test="long-description">
						<xsl:text>out &lt;&lt; "</xsl:text>
						<xsl:call-template name="escapeC">
							<xsl:with-param name="s" select="long-description/text()"/>
						</xsl:call-template>
						<xsl:text>";</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:message terminate="no">description missing for <xsl:value-of select="@name"/></xsl:message>
					</xsl:otherwise>
				 </xsl:choose>
				out &lt;&lt; std::endl;
			</xsl:for-each>
			return out;
			}

		virtual int main(int argc,char** argv) {
			if(argc==1) {
				usage(std::cerr);
				return EXIT_FAILURE;
				}
			if(std::strcmp("--help",argv[1])==0 || std::strcmp("-h",argv[1])==0 || std::strcmp("-?",argv[1])==0) {
				usage(std::cout);
				return EXIT_SUCCESS;
				}
			if(std::strcmp("--version",argv[1])==0) {
				std::cout &lt;&lt; program_version() &lt;&lt; std::endl;
				return EXIT_SUCCESS;
				}

<xsl:for-each select="program">
	<xsl:if test="position()!=1">else </xsl:if>
	<xsl:text>if(std::strcmp(argv[1],"</xsl:text>
	<xsl:value-of select="@name"/>
	<xsl:text>")==0) {
			try {
			return main_</xsl:text>
	 <xsl:value-of select="@name"/>
	<xsl:text>(argc-1,&amp;argv[1]);
				}
			catch(std::exception&amp; e) {
				THROW_PROGRAM_ERROR("[ERROR] </xsl:text><xsl:value-of select="@name"/><xsl:text> : " &lt;&lt; e.what());
				return EXIT_FAILURE;
				}
			catch(...) {
				THROW_PROGRAM_ERROR("[ERROR] </xsl:text><xsl:value-of select="@name"/><xsl:text>.");
				return EXIT_FAILURE;
				}
			
			}</xsl:text>
	</xsl:for-each>
	else {
		std::cerr &lt;&lt; "Unknown program \""  &lt;&lt; argv[1]  &lt;&lt; "\". Available programs are: "  &lt;&lt; std::endl;
		<xsl:for-each select="program[not(@hidden='false')]">
                         std::cerr &lt;&lt; "\t<xsl:value-of select="@name"/>\n";
		</xsl:for-each>
		return EXIT_FAILURE;
		}

			return EXIT_FAILURE;
			}

	};

#define PROGRAM_SHOW_HELP_OR_VERSION(args) \
	do {\
	if(args.show_help) {\
                args.usage(std::cout);\
                return EXIT_SUCCESS;\
                }\
        if(args.show_version) {\
		std::cout &lt;&lt; args.program_name() &lt;&lt; std::endl;\
                std::cout &lt;&lt; "Version: " &lt;&lt; args.program_version() &lt;&lt; std::endl;\
                std::cout &lt;&lt; "Git" &lt;&lt; GIT_VERSION &lt;&lt; std::endl;\
                return EXIT_SUCCESS;\
                }\
	}while(false)

#define PROGRAM_COMMON(args) \
	do {\
	PROGRAM_SHOW_HELP_OR_VERSION(args);\
        if(!args.validate()) {\
                return EXIT_FAILURE;\
                }\
	}while(false)

#undef THROW_PROGRAM_ERROR

</xsl:template>


<xsl:template match="program">
<xsl:variable name="programName">
    <xsl:value-of select="translate(substring(@name,1,1),'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ')"/>
    <xsl:value-of select="substring(@name,2)"/>
</xsl:variable>
<xsl:variable name="className">
    <xsl:value-of select="$programName"/>
    <xsl:text>Args</xsl:text>
</xsl:variable>

extern int main_<xsl:value-of select="@name"/>(int argc,char** argv);

class <xsl:value-of select="$className"/> : public ProgramArgs {
	public:
		<xsl:for-each select="option">
        <xsl:if test="short-description">
        /* <xsl:value-of select="short-description"/> */
        </xsl:if>
		<xsl:choose>
		  <xsl:when test="@type='bool'">bool</xsl:when>
          <xsl:when test="@type='char'">char</xsl:when>
          <xsl:when test="@type='string'">char*</xsl:when>
          <xsl:when test="@type='int'">int32_t</xsl:when>
		  <xsl:otherwise>
				<xsl:message terminate="yes">Class Declaration: Cannot handle <xsl:value-of select="@type"/></xsl:message>
		  </xsl:otherwise>
		</xsl:choose>
		<xsl:text> </xsl:text>
		<xsl:value-of select="@name"/>;
		</xsl:for-each>

	    <xsl:apply-templates select="code[@section='class-declaration']"/>


        /** constructor for <xsl:value-of select="$className"/>  */
		<xsl:value-of select="$className"/>() {
            /* initialize fields */
		<xsl:for-each select="option">
		<xsl:choose>
		  <xsl:when test="@type='bool'"><xsl:value-of select="@name"/> = false ;</xsl:when>
          <xsl:when test="@type='string' and @default-value"><xsl:value-of select="@name"/> = strdup(<xsl:value-of select='@default-value'/>); if(<xsl:value-of select="@name"/>==NULL) abort(); </xsl:when>
         <xsl:when test="@type='string'"><xsl:value-of select="@name"/> = NULL;</xsl:when>
         <xsl:when test="@default-value"><xsl:value-of select="@name"/> = (<xsl:value-of select="@type"/>)<xsl:value-of select='@default-value'/>;</xsl:when>
		</xsl:choose>
		</xsl:for-each>

	    <xsl:apply-templates select="code[@section='class-constructor']"/>
			}

		/** destructor for <xsl:value-of select="$className"/> */
		virtual ~<xsl:value-of select="$className"/>() {
            <xsl:for-each select="option">
            <xsl:choose>
		        <xsl:when test="@type='string'">if(<xsl:value-of select="@name"/> != NULL) {std::free(<xsl:value-of select="@name"/>);}</xsl:when>
            </xsl:choose>
		    </xsl:for-each>

	    <xsl:apply-templates select="code[@section='class-destructor']"/>

			}
    
		virtual const char* program_version() {
			<xsl:choose>
				<xsl:when test="@version">return "<xsl:value-of select="@version"/>";</xsl:when>
				<xsl:when test="../@version">return "<xsl:value-of select="../@version"/>";</xsl:when>
				<xsl:otherwise>return "0.0.0";</xsl:otherwise>
			</xsl:choose>
			}


		virtual const char* program_name() {        
            return "<xsl:value-of select="$programName"/>";
            }

		/** print usage */
		virtual std::ostream&amp; usage(std::ostream&amp; out) {
                out &lt;&lt; program_name() &lt;&lt; std::endl;
		out &lt;&lt; "Version: " &lt;&lt; program_version() &lt;&lt; std::endl;
		out &lt;&lt; "git: " &lt;&lt; GIT_VERSION &lt;&lt; std::endl;
		out &lt;&lt; "Author: Pierre Lindenbaum PhD" &lt;&lt; std::endl;
		out &lt;&lt; "Compilation: " &lt;&lt; __DATE__ &lt;&lt; " at " &lt;&lt; __TIME__ &lt;&lt; std::endl;

		out &lt;&lt; "Option: " &lt;&lt; std::endl;

			 <xsl:for-each select="option[not(@hidden) or not(@hidden='false')]">
				 out &lt;&lt; "\t" &lt;&lt; "<xsl:apply-templates select="." mode="usage"/>"  &lt;&lt; "  ";
				<xsl:choose>
					<xsl:when test="@type = 'bool'"></xsl:when>
					<xsl:when test="@arg-name">out &lt;&lt; "&lt;<xsl:value-of select="@arg-name"/>&gt;";</xsl:when>
					<xsl:otherwise>out &lt;&lt; "&lt;ARG&gt;";</xsl:otherwise>
				</xsl:choose>
				out &lt;&lt; "  " ;
				<xsl:choose>
					<xsl:when test="short-description">
						<xsl:text>out &lt;&lt; "</xsl:text>
						<xsl:call-template name="escapeC">
							<xsl:with-param name="s" select="short-description/text()"/>
						</xsl:call-template>
						<xsl:text>";</xsl:text>
					</xsl:when>
					<xsl:when test="long-description">
						<xsl:text>out &lt;&lt; "</xsl:text>
						<xsl:call-template name="escapeC">
							<xsl:with-param name="s" select="long-description/text()"/>
						</xsl:call-template>
						<xsl:text>";</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:message terminate="yes">description missing for <xsl:value-of select="@name"/></xsl:message>
					</xsl:otherwise>
				 </xsl:choose>
				out &lt;&lt; std::endl;
			</xsl:for-each>
			out &lt;&lt; std::endl;

	    <xsl:apply-templates select="code[@section='usage']"/>


	            return out;
			}

		/** parse arguments */
		virtual bool parse(int argc,char** argv) {
			/** https://stackoverflow.com/questions/19940100 */
			optind = 1;
  for(;;)  {
      option long_options[] = {
	<xsl:for-each select="option">
		{<xsl:choose>
			<xsl:when test="@long-option">
				<xsl:text>"</xsl:text>
				<xsl:value-of select="@long-option"/>
				<xsl:text>"</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>NULL</xsl:text>
			</xsl:otherwise>
		</xsl:choose>,<xsl:choose>
			<xsl:when test="@type = 'bool'">no_argument</xsl:when>
			<xsl:otherwise>required_argument</xsl:otherwise>
		</xsl:choose>,0,<xsl:choose>
                        <xsl:when test="@short-option">
				<xsl:text>'</xsl:text>
				<xsl:value-of select="@short-option"/>
				<xsl:text>'</xsl:text>
			</xsl:when>
                        <xsl:otherwise><xsl:value-of select="(position() + 999)"/></xsl:otherwise>
                </xsl:choose>},		
	</xsl:for-each>
		{0,0,0,0}
		};
	<xsl:for-each select="option[@shortOpt]">
		cmd+= '<xsl:value-of select="@shortOpt"/>;
                <xsl:if test="@type!='bool'">
                cmd+=':';
                </xsl:if>
	</xsl:for-each>

        
      /* getopt_long stores the option index here. */
      int option_index = 0;

      int c = ::getopt_long(argc, argv,"<xsl:for-each select="option[@short-option]">
		<xsl:value-of select="@short-option"/>
			<xsl:if test="@type!='bool'">
                		<xsl:text>:</xsl:text>
	                </xsl:if>
		</xsl:for-each>",long_options, &amp;option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;
      /** this is a long option */
      if (c==0) c= long_options[option_index].val;


      switch (c)
        {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;

	<xsl:for-each select="option">
	    <xsl:text>case </xsl:text>
	    <xsl:value-of select="(position() + 999)"/>
	    <xsl:text>: </xsl:text>
	    <xsl:if test="@short-option">
	    	<xsl:text>case '</xsl:text>
	        <xsl:value-of select="@short-option"/>
        	<xsl:text>' : </xsl:text>
	    </xsl:if>
		<xsl:choose>
			<xsl:when test="@type='bool'">
				 <xsl:text>this-&gt;</xsl:text>
                 <xsl:value-of select="@name"/>
                 <xsl:text> = true;</xsl:text>
			</xsl:when>
            <xsl:when test="@type='int' or @type='char'">
			 <xsl:text>try {
		this-&gt;</xsl:text>
                <xsl:value-of select="@name"/>
		<xsl:text> = </xsl:text>
		<xsl:value-of select="@parser"/>
		<xsl:text>(optarg); }
		catch(...) {
			THROW_PROGRAM_ERROR("Cannot parse \"" &lt;&lt; optarg &lt;&lt; " for </xsl:text>
			<xsl:apply-templates select="." mode="usage"/>
			<xsl:text>");
			std::exit(EXIT_FAILURE);
			}
		break;
		</xsl:text>
	    </xsl:when>
            <xsl:when test="@type='string'">
                <xsl:text>if(this-&gt;</xsl:text>
                <xsl:value-of select="@name"/>
                <xsl:text>!=NULL) {std::free(</xsl:text>
                <xsl:text>this-&gt;</xsl:text>
                <xsl:value-of select="@name"/>
				<xsl:text>) ;} this-&gt;</xsl:text>
                <xsl:value-of select="@name"/>
                <xsl:text>= strdup(optarg);</xsl:text>
		</xsl:when>
		

			<xsl:otherwise>
				<xsl:message terminate="yes">Switch/shortopt: cannot handle @type=<xsl:value-of select="@type"/></xsl:message>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>break;
        </xsl:text>
        </xsl:for-each>
	case ':' : {
		if(std::isprint(optopt)) {
			THROW_PROGRAM_ERROR("missing argument for option -" &lt;&lt; (char)optopt &lt;&lt;"'.");
			}
		else
			{
			THROW_PROGRAM_ERROR("missing arggument for " &lt;&lt; (int)optopt  &lt;&lt; ").");
			}
		return false;
		};
        case '?': {
		if(std::isprint(optopt)) {
			THROW_PROGRAM_ERROR("unknown option '" &lt;&lt; (char)optopt &lt;&lt;"'.");
			}
		else
			{
			THROW_PROGRAM_ERROR("unknown option (" &lt;&lt; (int)optopt  &lt;&lt; ").");
			}
		return false;
		}
          break;
        default:
          	THROW_PROGRAM_ERROR("strange option state.");
		return false;
        }
    }			
    while(optind &lt; argc) {
		this-&gt;program_files.push_back(argv[optind]);
		optind++;
		}
return true;	
}

       virtual bool validate() {
		bool ok=true;
            <xsl:for-each select="option">
			<xsl:if test="@type='string' and @required='true'">
			if(this-&gt;<xsl:value-of select="@name"/>==NULL) {
				ok = false;
				std::cerr &lt;&lt; program_name() &lt;&lt; " :<xsl:apply-templates select="." mode="usage"/> is required." &lt;&lt; std::endl;
				}
			</xsl:if>
			<xsl:if test="(@type='int' or @type='long') and @min-inclusive">
			if(this-&gt;<xsl:value-of select="@name"/> &lt; (<xsl:value-of select="@type"/>)<xsl:value-of select="@min-inclusive"/>) {
				ok = false;
				std::cerr &lt;&lt; program_name() &lt;&lt; " :<xsl:apply-templates select="." mode="usage"/> =" &lt;&lt; 
					this-&gt;<xsl:value-of select="@name"/>  &lt;&lt; 
					" should be greater or equal to <xsl:value-of select="@min-inclusive"/>." &lt;&lt; 
					std::endl;
				}
			</xsl:if>
            </xsl:for-each>
	    <xsl:apply-templates select="code[@section='validation']"/>
            return ok;
            }
	};



</xsl:template>

<xsl:template match="option" mode="usage">
	<xsl:choose>
		<xsl:when test="@short-option and @long-option">
			<xsl:text>-</xsl:text>
			<xsl:value-of select="@short-option"/>
			<xsl:text>|--</xsl:text>
			<xsl:value-of select="@long-option"/>
		</xsl:when>
		<xsl:when test="@short-option">
			<xsl:text>-</xsl:text>
			<xsl:value-of select="@short-option"/>
		</xsl:when>
		<xsl:when test="@long-option">
			<xsl:text>--</xsl:text>
			<xsl:value-of select="@long-option"/>
		</xsl:when>
		<xsl:otherwise>
			<xsl:message terminate="yes">no short or long option</xsl:message>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match="code">

/* BEGIN CUSTOM USER CODE */
<xsl:copy>
<xsl:apply-templates select="text()"/>
</xsl:copy>
/* END CUSTOM USER CODE */

</xsl:template>

<xsl:template name="escapeC">
<xsl:param name="s"/>
<xsl:value-of select="$s"/>
</xsl:template>




</xsl:stylesheet>
