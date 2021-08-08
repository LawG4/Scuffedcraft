LOCAL=$(dirname "$0")"/"
REPO_ROOT="${LOCAL}../"
source ${LOCAL}common.sh

cd ${REPO_ROOT}
make 
cd -