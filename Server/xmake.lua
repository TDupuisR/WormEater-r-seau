set_project("WormEater_server")

set_encodings("utf-8")
set_exceptions("cxx")
set_languages("c89", "cxx20")

add_requires("enet6")

if is_plat("windows") then

else
    add_cxxflags("-g3", "-O2","-Wall", "-Wextra")
end

target("WormEaterServer")
    set_kind("binary")

    add_headerfiles("sv_**.hpp")
    add_files("sv_**.cpp")
    add_packages("enet6")

    if is_plat("windows") then
        add_syslinks("ws2_32")
    else

    end
