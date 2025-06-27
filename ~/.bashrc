# CMake target completion
_cmake_target_completion() {
    local cur=${COMP_WORDS[COMP_CWORD]}
    COMPREPLY=( $(compgen -W "$(cmake --build . --target help 2>&1 | grep -v '^\.\.\.' | awk 'NR>1 {print $1}')" -- "$cur") )
}
complete -F _cmake_target_completion cmake