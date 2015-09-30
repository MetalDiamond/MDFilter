#ifndef KER_H
#define KER_H

/**
 * \brief 	Kernel element type
 */
#define KTYPE double

/**
 * \struct 	ker_t
 * \brief 	Kernel data structure
 */
typedef struct _ker_t {
    KTYPE **array; 	/**< The data array. */
    unsigned int height; 	/**< The number of rows. */
    unsigned int width; 	/**< The number of columns. */
} ker_t;


/**
 * \fn 		ker_t ker_init( unsigned char length )
 * \brief 	Kernel structure initialization function
 * \param length number of beans.
 * \return 	initialized kernel.
 *
 * Do not forget to destroy this structure.
 *
 */
ker_t *ker_init(int height, int width );


/**
 * \fn 		void ker_destroy(ker_t **)
 * \brief 	Kernel structure deallocation function
 * \param ker	Pointer on kernel pointer.
 * \return 	void
 *
 *
 */
void ker_destroy(ker_t **ker);


/**
 * \fn 		void ker_print(ker_t *ker)
 * \brief 	Print a kernel
 * \param ker	Pointer on the kernel to print.
 * \return 	void
 *
 *
 */
void ker_print(ker_t *ker);


/**
 * \fn 		ker_t *ker_gauss(int size, double sigma)
 * \brief 	Generate a centered symmetric gaussian kernel.
 * \param size	Size of the kernel.
 * \param sigma	Standard deviasion of the distribution.
 * \return 	Pointer on the generated kernel
 *
 *
 */
ker_t *ker_gauss(int size, double sigma);


/**
 * \fn 		void ker_normalize(ker_t *ker)
 * \brief 	Set the sum of the kernel to 1.
 * \param ker	Pointer on the kernel to process.
 * \return 	void
 *
 *
 */
void ker_normalize(ker_t *ker);

#endif // KER_H
